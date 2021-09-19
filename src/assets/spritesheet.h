#pragma once

#include "sprite.h"
#include "content.h"

#include <tinysdl.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <string>



namespace MicroNinja
{
	class SpriteSheet
	{

    public:

        struct Frame : Sprite {
            float delay_ms;
        };

        struct Animation {
            std::vector<Frame> frames;
            bool loop = true;

            size_t size() { return frames.size(); }
            float lenght()
            {
                float t = 0.0f;
                for (const auto& frame : frames)
                    t += frame.delay_ms;
                return t / 1000.0f;
            }
        };

		SpriteSheet() = default;
        SpriteSheet(const nlohmann::json& json, const std::string& key)
        {
            using namespace TinySDL;

            std::vector<Frame> frames;
            for (auto& frame : json["frames"])
            {
                frames.push_back({ TexRegion(Content::find<Texture>(key),
                                   Rect(frame["frame"]["x"], frame["frame"]["y"], frame["frame"]["w"], frame["frame"]["h"])), 
                                   {json["meta"]["slices"][0]["keys"][0]["pivot"]["x"], json["meta"]["slices"][0]["keys"][0]["pivot"]["y"]},
                                   frame["duration"]});
            }

            for (auto& tag : json["meta"]["frameTags"])
            {
                animations[tag["name"]] = Animation();

                for (size_t i = tag["from"]; i <= tag["to"]; i++)
                {
                    animations[tag["name"]].frames.push_back(frames[i]);
                }
            }
        }

        std::unordered_map<std::string, Animation> animations = {};
	};



}