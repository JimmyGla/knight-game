#include "raylib.h"
#include "raymath.h"
#include "character.h"
#include "prop.h"
#include "enemy.h"
#include <string>

main()
{
  // window dimensions
  int windowDimensions[2];
  windowDimensions[0] = 384;
  windowDimensions[1] = 384;

  // initialize the window
  InitWindow(windowDimensions[0], windowDimensions[1], "Snow Top Down");

  // load map texture
  Texture2D map = LoadTexture("nature_tileset/WorldMap.png");
  Vector2 mapPos{0.0f, 0.0f};
  const float mapScale{4.f};

  Character knight{windowDimensions[0], windowDimensions[1]};

  Prop props[]{
      Prop(Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")),
      Prop(Vector2{500.f, 4000.f}, LoadTexture("nature_tileset/Bush.png")),
      Prop(Vector2{4000.f, 500.f}, LoadTexture("nature_tileset/Sign.png")),

      Prop(Vector2{9000.f, 6000.f}, LoadTexture("nature_tileset/Log.png")),
      Prop(Vector2{1000.f, 800.f}, LoadTexture("nature_tileset/Bush.png")),
      Prop(Vector2{7000.f, 7000.f}, LoadTexture("nature_tileset/Rock.png"))};

  Enemy goblin{Vector2{800.f, 300.f},
               LoadTexture("characters/goblin_idle_spritesheet.png"),
               LoadTexture("characters/goblin_run_spritesheet.png")};

  Enemy slime{
      Vector2{500.f, 700.f},
      LoadTexture("characters/slime_idle_spritesheet.png"),
      LoadTexture("characters/slime_run_spritesheet.png")};

  Enemy *enemies[]{
      &goblin,
      &slime};

  for (auto enemy : enemies)
  {
    enemy->setTarget(&knight);
  }

  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(WHITE);

    mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

    // draw the map
    DrawTextureEx(map, mapPos, 0.0f, mapScale, WHITE);

    // draw the props
    for (auto prop : props)
    {
      prop.Render(knight.getWorldPos());
    }

    if (!knight.getAlive()) // character is not alive
    {
      DrawText("Game Over!", 55.f, 45.f, 40, RED);
      EndDrawing();
      continue;
    }
    else // character is alive
    {
      std::string knightsHealth = "Health: ";
      knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
      DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
    }

    knight.tick(GetFrameTime());
    if (knight.getWorldPos().x < 0.f ||
        knight.getWorldPos().y < 0.f ||
        knight.getWorldPos().x + windowDimensions[0] > map.width * mapScale ||
        knight.getWorldPos().y + windowDimensions[1] > map.height * mapScale)
    {
      // check map bounds
      knight.undoMovement();
    }

    // check prop collision
    for (auto prop : props)
    {
      if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
      {
        knight.undoMovement();
      }
    }

    for (auto enemy : enemies)
    {
      enemy->tick(GetFrameTime());
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
      for (auto enemy : enemies)
      {
        if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getCollisionRec()))
        {
          enemy->setAlive(false);
        }
      }
    }

    EndDrawing();
  }
  UnloadTexture(map);
  CloseWindow();
}