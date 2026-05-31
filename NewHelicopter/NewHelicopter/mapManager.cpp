#pragma once

#include "MapManager.h"
#include "Platform.h"
#include "Utillity.h"
#include "OiiAiGame.h"
#include <cmath>

MapManager::MapManager()
{
    std::random_device rd;
    m_randomEngine.seed(rd());
}

MapManager::~MapManager()
{
    Clear();
}

void MapManager::Init(OiiAGame* game)
{
    m_game = game;

    Clear();
    RegisterChunkPatterns();

    GenerateChunk(0, 0);
    GenerateChunk(0, -1);
}

void MapManager::Update(const learning::Vector2f& playerPos, const learning::Vector2f& playerDir)
{
    GenerateAroundPlayer(playerPos, playerDir);
    RemoveFarChunks(playerPos);
}

void MapManager::Clear()
{
    for (auto& pair : m_generatedChunks)
    {
        GeneratedChunk& chunk = pair.second;

        for (Platform* platform : chunk.platforms)
        {
            DestroyPlatform(platform);
        }

        chunk.platforms.clear();
    }

    m_generatedChunks.clear();
    m_patterns.clear();
}

bool MapManager::CanGenerateChunkX(int chunkX) const
{
    float chunkLeft = chunkX * CHUNK_W;
    float chunkRight = chunkLeft + CHUNK_W;

    if (chunkRight < WORLD_MIN_X)
        return false;

    if (chunkLeft > WORLD_MAX_X)
        return false;

    return true;
}

void MapManager::RegisterChunkPatterns()
{
    m_patterns.push_back(chunkA);
    m_patterns.push_back(chunkB);
    m_patterns.push_back(chunkC);
    m_patterns.push_back(chunkD);
    m_patterns.push_back(chunkE);
    m_patterns.push_back(chunkF);
    m_patterns.push_back(chunkG);
}

void MapManager::GenerateAroundPlayer(const learning::Vector2f& playerPos, const learning::Vector2f& playerDir)
{
    ChunkCoord current = WorldToChunkCoord(playerPos);

    int dirX = 0;

    if (playerDir.x > 0.4f)
        dirX = 1;
    else if (playerDir.x < -0.4f)
        dirX = -1;

    for (int y = -PRELOAD_RANGE_Y; y <= 0; ++y)
    {
        for (int x = -PRELOAD_RANGE_X; x <= PRELOAD_RANGE_X; ++x)
        {
            GenerateChunk(current.x + x + dirX, current.y + y);
        }
    }
}
void MapManager::GenerateChunk(int chunkX, int chunkY)
{
    if (!CanGenerateChunkX(chunkX))
        return;

    ChunkCoord coord{ chunkX, chunkY };

    if (m_generatedChunks.find(coord) != m_generatedChunks.end())
        return;

    const MapChunkPattern& pattern = GetRandomChunkPattern();

    GeneratedChunk generated;
    generated.coord = coord;

    float chunkWorldX = chunkX * CHUNK_W;
    float chunkWorldY = chunkY * CHUNK_H;

    for (int row = 0; row < CHUNK_ROW; ++row)
    {
        for (int col = 0; col < CHUNK_COL; ++col)
        {
            if (pattern.data[row][col] != 1)
                continue;

            float worldX = chunkWorldX + col * PLATFORM_W;

            if (worldX < WORLD_MIN_X || worldX > WORLD_MAX_X)
                continue;

            float worldY = chunkWorldY + row * PLATFORM_H;

            Platform* platform = CreatePlatform(worldX, worldY);

            if (platform != nullptr)
                generated.platforms.push_back(platform);
        }
    }

    m_generatedChunks.insert({ coord, generated });
}

void MapManager::RemoveFarChunks(const learning::Vector2f& playerPos)
{
    ChunkCoord current = WorldToChunkCoord(playerPos);

    std::vector<ChunkCoord> removeList;

    for (const auto& pair : m_generatedChunks)
    {
        const ChunkCoord& coord = pair.first;

        int dx = std::abs(coord.x - current.x);
        int dy = std::abs(coord.y - current.y);

        if (dx > REMOVE_RANGE_X || dy > REMOVE_RANGE_Y)
            removeList.push_back(coord);
    }

    for (const ChunkCoord& coord : removeList)
    {
        auto iter = m_generatedChunks.find(coord);

        if (iter == m_generatedChunks.end())
            continue;

        GeneratedChunk& chunk = iter->second;

        for (Platform* platform : chunk.platforms)
        {
            DestroyPlatform(platform);
        }

        chunk.platforms.clear();
        m_generatedChunks.erase(iter);
    }
}

const MapChunkPattern& MapManager::GetRandomChunkPattern()
{
    std::uniform_int_distribution<int> dist(
        0,
        static_cast<int>(m_patterns.size()) - 1
    );

    return m_patterns[dist(m_randomEngine)];
}

ChunkCoord MapManager::WorldToChunkCoord(const learning::Vector2f& worldPos) const
{
    int chunkX = static_cast<int>(std::floor(worldPos.x / CHUNK_W));
    int chunkY = static_cast<int>(std::floor(worldPos.y / CHUNK_H));

    return ChunkCoord{ chunkX, chunkY };
}

Platform* MapManager::CreatePlatform(float worldX, float worldY)
{
    if (m_game == nullptr)
        return nullptr;

    return m_game->CreatePlatform(worldX, worldY);
}

void MapManager::DestroyPlatform(Platform* platform)
{
    if (m_game == nullptr || platform == nullptr)
        return;

    m_game->DestroyObject(platform);
}