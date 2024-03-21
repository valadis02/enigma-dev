/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

using namespace std;

int lang_CPP::module_write_backgrounds(const GameData &game, ofstream &gameModule)
{
    // Write the magic number
    gameModule << "BKG ";
    
    // Write the number of backgrounds
    int back_count = game.backgrounds.size();
    gameModule.write(reinterpret_cast<const char*>(&back_count), sizeof(back_count));

    // Write the maximum background ID
    int back_maxid = 0;
    for (const auto& background : game.backgrounds) {
        if (background.id() > back_maxid) {
            back_maxid = background.id();
        }
    }
    gameModule.write(reinterpret_cast<const char*>(&back_maxid), sizeof(back_maxid));

    // Write background data dynamically
    for (const auto& background : game.backgrounds) {
        // Write background ID
        gameModule.write(reinterpret_cast<const char*>(&background.id()), sizeof(background.id()));

        // Write image dimensions
        gameModule.write(reinterpret_cast<const char*>(&background.image_data.width), sizeof(background.image_data.width));
        gameModule.write(reinterpret_cast<const char*>(&background.image_data.height), sizeof(background.image_data.height));

        // Write other attributes dynamically
        vector<int> attributes = {
            background.legacy_transparency,
            background->smooth_edges(),
            background->preload(),
            background->use_as_tileset(),
            background->tile_width(),
            background->tile_height(),
            background->horizontal_offset(),
            background->vertical_offset(),
            background->horizontal_spacing(),
            background->vertical_spacing()
        };
        for (const auto& attr : attributes) {
            gameModule.write(reinterpret_cast<const char*>(&attr), sizeof(attr));
        }

        // Write pixel data size
        int pixel_size = background.image_data.pixels.size();
        gameModule.write(reinterpret_cast<const char*>(&pixel_size), sizeof(pixel_size));

        // Write pixel data
        gameModule.write(reinterpret_cast<const char*>(background.image_data.pixels.data()), pixel_size);
    }

    return 0;
}
