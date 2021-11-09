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
                Rect frame_rect;
                Vec2 pivot;
                
                if(json["meta"]["slices"].size() < 1)
                {
                    pivot = Vec2::zeros;

                    frame_rect.x = frame["frame"]["x"];
                    frame_rect.y = frame["frame"]["y"];
                    frame_rect.w = frame["frame"]["w"];
                    frame_rect.h = frame["frame"]["h"];
                }
                else
                {
                    auto& slice = json["meta"]["slices"][0]["keys"][0];

                    pivot = {(float) slice["pivot"]["x"], (float) slice["pivot"]["y"]};

                    frame_rect.x = frame["frame"]["x"] + (float) slice["bounds"]["x"];
                    frame_rect.y = frame["frame"]["y"] + (float) slice["bounds"]["y"];
                    frame_rect.w = slice["bounds"]["w"];
                    frame_rect.h = slice["bounds"]["h"];
                }

                frames.push_back({ TexRegion(Content::find<Texture>(key), frame_rect),
                                   pivot.cast_to<int>(), frame["duration"]});
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