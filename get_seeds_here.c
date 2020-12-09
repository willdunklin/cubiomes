#include "finders.h"
#include "generator.h"
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

int dist(int x1, int z1, int x2, int z2)
{
    int dx = x1 - x2;
    int dz = z1 - z2;
    return (int) sqrt(dx * dx + dz * dz);
}

// Finds the distance to the closest structure to Pos s
int close_struct(int range, StructureConfig conf, int structure, int64_t seed, int version, LayerStack *g, int radius, Pos s)
{
    int x, z;
    int min_dist = 9999;
    Pos pos;

    // Go through regions in range
    for(x = -range; x < range; ++x)
        for(z = -range; z < range; ++z) 
        {
            pos = getStructurePos(conf, seed, x, z, NULL);
            // Local minimum distance
            if(isViableStructurePos(structure, version, g, seed, pos.x, pos.z) && min_dist > dist(pos.x, pos.z, s.x, s.z))
                min_dist = dist(pos.x, pos.z, s.x, s.z);
        }

    if(min_dist < radius) 
        return min_dist;
    return -1;
}

// This is very slow be warned
int close_biome(LayerStack *g, int biome, int stride, int radius, Pos s)
{
    int x, z, biomeID;
    for(x = s.x - radius; x < s.x + radius; x += stride) {
        for(z = s.z - radius; z < s.z + radius; z += stride) {
            Pos pos = {x, z};
            biomeID = getBiomeAtPos(g, pos);
            if(biomeID == biome)
                return dist(x, z, s.x, s.z);
        }
    }
    return -1;
}

int main()
{
    int version = MC_1_16;
    initBiomes();

    LayerStack g;
    setupGenerator(&g, version);

    int64_t seed;

    Pos spawn = {0, 0};
    Pos zero = {0, 0};

    printf("Seed: distances to structures\n");
    
    // for (seed = 1;; ++seed)
    for (seed = time(NULL);; nextLong(&seed))
    {
        applySeed(&g, seed);

        // Getting the actual spawn is slow as fuck be warned
        // spawn = getSpawn(version, &g, NULL, seed);

        // List of structure configs is in finders.h line 100                           | This is the radius the structure needs to be within
        // Enun of structure types is in finders.h line 34                              v
        int bastion_dist = close_struct(1, BASTION_CONFIG, Bastion, seed, version, &g, 200, zero);
        int fortress_dist = close_struct(1, FORTRESS_CONFIG, Fortress, seed, version, &g, 200, zero);
        int village_dist = close_struct(1, VILLAGE_CONFIG, Village, seed, version, &g, 150, spawn);
        int monument_dist = close_struct(1, MONUMENT_CONFIG, Monument, seed, version, &g, 300, spawn);

        // Biomes are also super slow so yeah
        // List of biomes is in layers.h line 29
        // int mushroom_dist = close_biome(&g, mushroomIsland, 32, 64, spawn);

        // Copy these to make more, comment out to exclude
        if (fortress_dist != -1)
        if (bastion_dist != -1)
        // if (village_dist != -1)
        if (monument_dist != -1)
        // if (mushroom_dist != -1)
        {
            printf("%" PRId64 ": %d %d %d %d\n", seed, fortress_dist, bastion_dist, village_dist, monument_dist);
        }
    }

    return 0;
}
