//  File generated with quicktype.io
//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     LDTk::File data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json & j, const std::shared_ptr<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json & j) {
            if (j.is_null()) return std::unique_ptr<T>(); else return std::unique_ptr<T>(new T(j.get<T>()));
        }
    };
}
#endif

namespace LDTk {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json & j, std::string property) {
        return get_optional<T>(j, property.data());
    }

    /**
     * Possible values: `Hidden`, `ValueOnly`, `NameAndValue`, `EntityTile`, `Points`,
     * `PointStar`, `PointPath`, `PointPathLoop`, `RadiusPx`, `RadiusGrid`
     */
    enum class EditorDisplayMode : int { ENTITY_TILE, HIDDEN, NAME_AND_VALUE, POINTS, POINT_PATH, POINT_PATH_LOOP, POINT_STAR, RADIUS_GRID, RADIUS_PX, VALUE_ONLY };

    /**
     * Possible values: `Above`, `Center`, `Beneath`
     */
    enum class EditorDisplayPos : int { ABOVE, BENEATH, CENTER };

    enum class TextLanguageMode : int { LANG_C, LANG_HAXE, LANG_JS, LANG_JSON, LANG_LUA, LANG_MARKDOWN, LANG_PYTHON, LANG_RUBY, LANG_XML };

    /**
     * This section is mostly only intended for the LDtk editor app itself. You can safely
     * ignore it.
     */
    struct FieldDefinition {
        /**
         * Human readable value type (eg. `Int`, `Float`, `Point`, etc.). If the field is an array,
         * this field will look like `Array<...>` (eg. `Array<Int>`, `Array<Point>` etc.)
         */
        std::string type;
        /**
         * Optional list of accepted file extensions for FilePath value type. Includes the dot:
         * `.ext`
         */
        std::shared_ptr<std::vector<std::string>> accept_file_types;
        /**
         * Array max length
         */
        std::shared_ptr<int64_t> array_max_length;
        /**
         * Array min length
         */
        std::shared_ptr<int64_t> array_min_length;
        /**
         * TRUE if the value can be null. For arrays, TRUE means it can contain null values
         * (exception: array of Points can't have null values).
         */
        bool can_be_null;
        /**
         * Default value if selected value is null or invalid.
         */
        json default_override;
        bool editor_always_show;
        bool editor_cut_long_values;
        /**
         * Possible values: `Hidden`, `ValueOnly`, `NameAndValue`, `EntityTile`, `Points`,
         * `PointStar`, `PointPath`, `PointPathLoop`, `RadiusPx`, `RadiusGrid`
         */
        LDTk::EditorDisplayMode editor_display_mode;
        /**
         * Possible values: `Above`, `Center`, `Beneath`
         */
        LDTk::EditorDisplayPos editor_display_pos;
        /**
         * Unique String identifier
         */
        std::string identifier;
        /**
         * TRUE if the value is an array of multiple values
         */
        bool is_array;
        /**
         * Max limit for value, if applicable
         */
        std::shared_ptr<double> max;
        /**
         * Min limit for value, if applicable
         */
        std::shared_ptr<double> min;
        /**
         * Optional regular expression that needs to be matched to accept values. Expected format:
         * `/some_reg_ex/g`, with optional "i" flag.
         */
        std::shared_ptr<std::string> regex;
        /**
         * Possible values: &lt;`null`&gt;, `LangPython`, `LangRuby`, `LangJS`, `LangLua`, `LangC`,
         * `LangHaxe`, `LangMarkdown`, `LangJson`, `LangXml`
         */
        std::shared_ptr<LDTk::TextLanguageMode> text_language_mode;
        /**
         * Internal type enum
         */
        json field_definition_type;
        /**
         * Unique Int identifier
         */
        int64_t uid;
    };

    /**
     * Possible values: `DiscardOldOnes`, `PreventAdding`, `MoveLastOne`
     */
    enum class LimitBehavior : int { DISCARD_OLD_ONES, MOVE_LAST_ONE, PREVENT_ADDING };

    /**
     * If TRUE, the maxCount is a "per world" limit, if FALSE, it's a "per level". Possible
     * values: `PerLayer`, `PerLevel`, `PerWorld`
     */
    enum class LimitScope : int { PER_LAYER, PER_LEVEL, PER_WORLD };

    /**
     * Possible values: `Rectangle`, `Ellipse`, `Tile`, `Cross`
     */
    enum class RenderMode : int { CROSS, ELLIPSE, RECTANGLE, TILE };

    /**
     * Possible values: `Cover`, `FitInside`, `Repeat`, `Stretch`
     */
    enum class TileRenderMode : int { COVER, FIT_INSIDE, REPEAT, STRETCH };

    struct EntityDefinition {
        /**
         * Base entity color
         */
        std::string color;
        /**
         * Array of field definitions
         */
        std::vector<LDTk::FieldDefinition> field_defs;
        double fill_opacity;
        /**
         * Pixel height
         */
        int64_t height;
        bool hollow;
        /**
         * Unique String identifier
         */
        std::string identifier;
        /**
         * Only applies to entities resizable on both X/Y. If TRUE, the entity instance width/height
         * will keep the same aspect ratio as the definition.
         */
        bool keep_aspect_ratio;
        /**
         * Possible values: `DiscardOldOnes`, `PreventAdding`, `MoveLastOne`
         */
        LDTk::LimitBehavior limit_behavior;
        /**
         * If TRUE, the maxCount is a "per world" limit, if FALSE, it's a "per level". Possible
         * values: `PerLayer`, `PerLevel`, `PerWorld`
         */
        LDTk::LimitScope limit_scope;
        double line_opacity;
        /**
         * Max instances count
         */
        int64_t max_count;
        /**
         * Pivot X coordinate (from 0 to 1.0)
         */
        double pivot_x;
        /**
         * Pivot Y coordinate (from 0 to 1.0)
         */
        double pivot_y;
        /**
         * Possible values: `Rectangle`, `Ellipse`, `Tile`, `Cross`
         */
        LDTk::RenderMode render_mode;
        /**
         * If TRUE, the entity instances will be resizable horizontally
         */
        bool resizable_x;
        /**
         * If TRUE, the entity instances will be resizable vertically
         */
        bool resizable_y;
        /**
         * Display entity name in editor
         */
        bool show_name;
        /**
         * An array of strings that classifies this entity
         */
        std::vector<std::string> tags;
        /**
         * Tile ID used for optional tile display
         */
        std::shared_ptr<int64_t> tile_id;
        /**
         * Possible values: `Cover`, `FitInside`, `Repeat`, `Stretch`
         */
        LDTk::TileRenderMode tile_render_mode;
        /**
         * Tileset ID used for optional tile display
         */
        std::shared_ptr<int64_t> tileset_id;
        /**
         * Unique Int identifier
         */
        int64_t uid;
        /**
         * Pixel width
         */
        int64_t width;
    };

    struct EnumValueDefinition {
        /**
         * An array of 4 Int values that refers to the tile in the tileset image: `[ x, y, width,
         * height ]`
         */
        std::shared_ptr<std::vector<int64_t>> tile_src_rect;
        /**
         * Optional color
         */
        int64_t color;
        /**
         * Enum value
         */
        std::string id;
        /**
         * The optional ID of the tile
         */
        std::shared_ptr<int64_t> tile_id;
    };

    struct EnumDefinition {
        std::shared_ptr<std::string> external_file_checksum;
        /**
         * Relative path to the external file providing this Enum
         */
        std::shared_ptr<std::string> external_rel_path;
        /**
         * Tileset UID if provided
         */
        std::shared_ptr<int64_t> icon_tileset_uid;
        /**
         * Unique String identifier
         */
        std::string identifier;
        /**
         * Unique Int identifier
         */
        int64_t uid;
        /**
         * All possible enum values, with their optional Tile infos.
         */
        std::vector<LDTk::EnumValueDefinition> values;
    };

    /**
     * Checker mode Possible values: `None`, `Horizontal`, `Vertical`
     */
    enum class Checker : int { HORIZONTAL, NONE, VERTICAL };

    /**
     * Defines how tileIds array is used Possible values: `Single`, `Stamp`
     */
    enum class TileMode : int { SINGLE, STAMP };

    /**
     * This complex section isn't meant to be used by game devs at all, as these rules are
     * completely resolved internally by the editor before any saving. You should just ignore
     * this part.
     */
    struct AutoLayerRuleDefinition {
        /**
         * If FALSE, the rule effect isn't applied, and no tiles are generated.
         */
        bool active;
        /**
         * When TRUE, the rule will prevent other rules to be applied in the same cell if it matches
         * (TRUE by default).
         */
        bool break_on_match;
        /**
         * Chances for this rule to be applied (0 to 1)
         */
        double chance;
        /**
         * Checker mode Possible values: `None`, `Horizontal`, `Vertical`
         */
        LDTk::Checker checker;
        /**
         * If TRUE, allow rule to be matched by flipping its pattern horizontally
         */
        bool flip_x;
        /**
         * If TRUE, allow rule to be matched by flipping its pattern vertically
         */
        bool flip_y;
        /**
         * Default IntGrid value when checking cells outside of level bounds
         */
        std::shared_ptr<int64_t> out_of_bounds_value;
        /**
         * Rule pattern (size x size)
         */
        std::vector<int64_t> pattern;
        /**
         * If TRUE, enable Perlin filtering to only apply rule on specific random area
         */
        bool perlin_active;
        double perlin_octaves;
        double perlin_scale;
        double perlin_seed;
        /**
         * X pivot of a tile stamp (0-1)
         */
        double pivot_x;
        /**
         * Y pivot of a tile stamp (0-1)
         */
        double pivot_y;
        /**
         * Pattern width & height. Should only be 1,3,5 or 7.
         */
        int64_t size;
        /**
         * Array of all the tile IDs. They are used randomly or as stamps, based on `tileMode` value.
         */
        std::vector<int64_t> tile_ids;
        /**
         * Defines how tileIds array is used Possible values: `Single`, `Stamp`
         */
        LDTk::TileMode tile_mode;
        /**
         * Unique Int identifier
         */
        int64_t uid;
        /**
         * X cell coord modulo
         */
        int64_t x_modulo;
        /**
         * Y cell coord modulo
         */
        int64_t y_modulo;
    };

    struct AutoLayerRuleGroup {
        bool active;
        bool collapsed;
        bool is_optional;
        std::string name;
        std::vector<LDTk::AutoLayerRuleDefinition> rules;
        int64_t uid;
    };

    /**
     * IntGrid value definition
     */
    struct IntGridValueDefinition {
        std::string color;
        /**
         * Unique String identifier
         */
        std::shared_ptr<std::string> identifier;
        /**
         * The IntGrid value itself
         */
        int64_t value;
    };

    /**
     * Type of the layer as Haxe Enum Possible values: `IntGrid`, `Entities`, `Tiles`,
     * `AutoLayer`
     */
    enum class Type : int { AUTO_LAYER, ENTITIES, INT_GRID, TILES };

    struct LayerDefinition {
        /**
         * Type of the layer (*IntGrid, Entities, Tiles or AutoLayer*)
         */
        std::string type;
        /**
         * Contains all the auto-layer rule definitions.
         */
        std::vector<LDTk::AutoLayerRuleGroup> auto_rule_groups;
        std::shared_ptr<int64_t> auto_source_layer_def_uid;
        /**
         * Reference to the Tileset UID being used by this auto-layer rules. WARNING: some layer
         * *instances* might use a different tileset. So most of the time, you should probably use
         * the `__tilesetDefUid` value from layer instances.
         */
        std::shared_ptr<int64_t> auto_tileset_def_uid;
        /**
         * Opacity of the layer (0 to 1.0)
         */
        double display_opacity;
        /**
         * An array of tags to forbid some Entities in this layer
         */
        std::vector<std::string> excluded_tags;
        /**
         * Width and height of the grid in pixels
         */
        int64_t grid_size;
        /**
         * Unique String identifier
         */
        std::string identifier;
        /**
         * An array that defines extra optional info for each IntGrid value. The array is sorted
         * using value (ascending).
         */
        std::vector<LDTk::IntGridValueDefinition> int_grid_values;
        /**
         * X offset of the layer, in pixels (IMPORTANT: this should be added to the `LayerInstance`
         * optional offset)
         */
        int64_t px_offset_x;
        /**
         * Y offset of the layer, in pixels (IMPORTANT: this should be added to the `LayerInstance`
         * optional offset)
         */
        int64_t px_offset_y;
        /**
         * An array of tags to filter Entities that can be added to this layer
         */
        std::vector<std::string> required_tags;
        /**
         * If the tiles are smaller or larger than the layer grid, the pivot value will be used to
         * position the tile relatively its grid cell.
         */
        double tile_pivot_x;
        /**
         * If the tiles are smaller or larger than the layer grid, the pivot value will be used to
         * position the tile relatively its grid cell.
         */
        double tile_pivot_y;
        /**
         * Reference to the Tileset UID being used by this Tile layer. WARNING: some layer
         * *instances* might use a different tileset. So most of the time, you should probably use
         * the `__tilesetDefUid` value from layer instances.
         */
        std::shared_ptr<int64_t> tileset_def_uid;
        /**
         * Type of the layer as Haxe Enum Possible values: `IntGrid`, `Entities`, `Tiles`,
         * `AutoLayer`
         */
        LDTk::Type layer_definition_type;
        /**
         * Unique Int identifier
         */
        int64_t uid;
    };

    /**
     * The `Tileset` definition is the most important part among project definitions. It
     * contains some extra informations about each integrated tileset. If you only had to parse
     * one definition section, that would be the one.
     */
    struct TilesetDefinition {
        /**
         * Grid-based height
         */
        int64_t c_hei;
        /**
         * Grid-based width
         */
        int64_t c_wid;
        /**
         * The following data is used internally for various optimizations. It's always synced with
         * source image changes.
         */
        std::shared_ptr<std::map<std::string, json>> cached_pixel_data;
        /**
         * An array of custom tile metadata
         */
        std::vector<std::map<std::string, json>> custom_data;
        /**
         * Tileset tags using Enum values specified by `tagsSourceEnumId`. This array contains 1
         * element per Enum value, which contains an array of all Tile IDs that are tagged with it.
         */
        std::vector<std::map<std::string, json>> enum_tags;
        /**
         * Unique String identifier
         */
        std::string identifier;
        /**
         * Distance in pixels from image borders
         */
        int64_t padding;
        /**
         * Image height in pixels
         */
        int64_t px_hei;
        /**
         * Image width in pixels
         */
        int64_t px_wid;
        /**
         * Path to the source file, relative to the current project JSON file
         */
        std::string rel_path;
        /**
         * Array of group of tiles selections, only meant to be used in the editor
         */
        std::vector<std::map<std::string, json>> saved_selections;
        /**
         * Space in pixels between all tiles
         */
        int64_t spacing;
        /**
         * Optional Enum definition UID used for this tileset meta-data
         */
        std::shared_ptr<int64_t> tags_source_enum_uid;
        int64_t tile_grid_size;
        /**
         * Unique Intidentifier
         */
        int64_t uid;
    };

    /**
     * A structure containing all the definitions of this project
     *
     * If you're writing your own LDtk importer, you should probably just ignore *most* stuff in
     * the `defs` section, as it contains data that are mostly important to the editor. To keep
     * you away from the `defs` section and avoid some unnecessary JSON parsing, important data
     * from definitions is often duplicated in fields prefixed with a double underscore (eg.
     * `__identifier` or `__type`).  The 2 only definition types you might need here are
     * **Tilesets** and **Enums**.
     */
    struct Definitions {
        /**
         * All entities definitions, including their custom fields
         */
        std::vector<LDTk::EntityDefinition> entities;
        /**
         * All internal enums
         */
        std::vector<LDTk::EnumDefinition> enums;
        /**
         * Note: external enums are exactly the same as `enums`, except they have a `relPath` to
         * point to an external source file.
         */
        std::vector<LDTk::EnumDefinition> external_enums;
        /**
         * All layer definitions
         */
        std::vector<LDTk::LayerDefinition> layers;
        /**
         * All custom fields available to all levels.
         */
        std::vector<LDTk::FieldDefinition> level_fields;
        /**
         * All tilesets
         */
        std::vector<LDTk::TilesetDefinition> tilesets;
    };

    enum class Flag : int { DISCARD_PRE_CSV_INT_GRID, IGNORE_BACKUP_SUGGEST };

    /**
     * "Image export" option when saving project. Possible values: `None`, `OneImagePerLayer`,
     * `OneImagePerLevel`
     */
    enum class ImageExportMode : int { NONE, ONE_IMAGE_PER_LAYER, ONE_IMAGE_PER_LEVEL };

    /**
     * Level background image position info
     */
    struct LevelBackgroundPosition {
        /**
         * An array of 4 float values describing the cropped sub-rectangle of the displayed
         * background image. This cropping happens when original is larger than the level bounds.
         * Array format: `[ cropX, cropY, cropWidth, cropHeight ]`
         */
        std::vector<double> crop_rect;
        /**
         * An array containing the `[scaleX,scaleY]` values of the **cropped** background image,
         * depending on `bgPos` option.
         */
        std::vector<double> scale;
        /**
         * An array containing the `[x,y]` pixel coordinates of the top-left corner of the
         * **cropped** background image, depending on `bgPos` option.
         */
        std::vector<int64_t> top_left_px;
    };

    struct FieldInstance {
        /**
         * Field definition identifier
         */
        std::string identifier;
        /**
         * Type of the field, such as `Int`, `Float`, `Enum(my_enum_name)`, `Bool`, etc.
         */
        std::string type;
        /**
         * Actual value of the field instance. The value type may vary, depending on `__type`
         * (Integer, Boolean, String etc.)<br/>  It can also be an `Array` of those same types.
         */
        json value;
        /**
         * Reference of the **Field definition** UID
         */
        int64_t def_uid;
        /**
         * Editor internal raw values
         */
        std::vector<json> real_editor_values;
    };

    /**
     * This structure represents a single tile from a given Tileset.
     */
    struct TileInstance {
        /**
         * Internal data used by the editor.<br/>  For auto-layer tiles: `[ruleId, coordId]`.<br/>
         * For tile-layer tiles: `[coordId]`.
         */
        std::vector<int64_t> d;
        /**
         * "Flip bits", a 2-bits integer to represent the mirror transformations of the tile.<br/>
         * - Bit 0 = X flip<br/>   - Bit 1 = Y flip<br/>   Examples: f=0 (no flip), f=1 (X flip
         * only), f=2 (Y flip only), f=3 (both flips)
         */
        int64_t f;
        /**
         * Pixel coordinates of the tile in the **layer** (`[x,y]` format). Don't forget optional
         * layer offsets, if they exist!
         */
        std::vector<int64_t> px;
        /**
         * Pixel coordinates of the tile in the **tileset** (`[x,y]` format)
         */
        std::vector<int64_t> src;
        /**
         * The *Tile ID* in the corresponding tileset.
         */
        int64_t t;
    };

    /**
     * Tile data in an Entity instance
     */
    struct EntityInstanceTile {
        /**
         * An array of 4 Int values that refers to the tile in the tileset image: `[ x, y, width,
         * height ]`
         */
        std::vector<int64_t> src_rect;
        /**
         * Tileset ID
         */
        int64_t tileset_uid;
    };

    struct EntityInstance {
        /**
         * Grid-based coordinates (`[x,y]` format)
         */
        std::vector<int64_t> grid;
        /**
         * Entity definition identifier
         */
        std::string identifier;
        /**
         * Pivot coordinates  (`[x,y]` format, values are from 0 to 1) of the Entity
         */
        std::vector<double> pivot;
        /**
         * Optional Tile used to display this entity (it could either be the default Entity tile, or
         * some tile provided by a field value, like an Enum).
         */
        std::shared_ptr<LDTk::EntityInstanceTile> tile;
        /**
         * Reference of the **Entity definition** UID
         */
        int64_t def_uid;
        /**
         * An array of all custom fields and their values.
         */
        std::vector<LDTk::FieldInstance> field_instances;
        /**
         * Entity height in pixels. For non-resizable entities, it will be the same as Entity
         * definition.
         */
        int64_t height;
        /**
         * Pixel coordinates (`[x,y]` format) in current level coordinate space. Don't forget
         * optional layer offsets, if they exist!
         */
        std::vector<int64_t> px;
        /**
         * Entity width in pixels. For non-resizable entities, it will be the same as Entity
         * definition.
         */
        int64_t width;
    };

    /**
     * IntGrid value instance
     */
    struct IntGridValueInstance {
        /**
         * Coordinate ID in the layer grid
         */
        int64_t coord_id;
        /**
         * IntGrid value
         */
        int64_t v;
    };

    struct LayerInstance {
        /**
         * Grid-based height
         */
        int64_t c_hei;
        /**
         * Grid-based width
         */
        int64_t c_wid;
        /**
         * Grid size
         */
        int64_t grid_size;
        /**
         * Layer definition identifier
         */
        std::string identifier;
        /**
         * Layer opacity as Float [0-1]
         */
        double opacity;
        /**
         * Total layer X pixel offset, including both instance and definition offsets.
         */
        int64_t px_total_offset_x;
        /**
         * Total layer Y pixel offset, including both instance and definition offsets.
         */
        int64_t px_total_offset_y;
        /**
         * The definition UID of corresponding Tileset, if any.
         */
        std::shared_ptr<int64_t> tileset_def_uid;
        /**
         * The relative path to corresponding Tileset, if any.
         */
        std::shared_ptr<std::string> tileset_rel_path;
        /**
         * Layer type (possible values: IntGrid, Entities, Tiles or AutoLayer)
         */
        std::string type;
        /**
         * An array containing all tiles generated by Auto-layer rules. The array is already sorted
         * in display order (ie. 1st tile is beneath 2nd, which is beneath 3rd etc.).<br/><br/>
         * Note: if multiple tiles are stacked in the same cell as the result of different rules,
         * all tiles behind opaque ones will be discarded.
         */
        std::vector<LDTk::TileInstance> auto_layer_tiles;
        std::vector<LDTk::EntityInstance> entity_instances;
        std::vector<LDTk::TileInstance> grid_tiles;
        /**
         * **WARNING**: this deprecated value will be *removed* completely on version 0.10.0+
         * Replaced by: `intGridCsv`
         */
        std::shared_ptr<std::vector<LDTk::IntGridValueInstance>> int_grid;
        /**
         * A list of all values in the IntGrid layer, stored from left to right, and top to bottom
         * (ie. first row from left to right, followed by second row, etc). `0` means "empty cell"
         * and IntGrid values start at 1. This array size is `__cWid` x `__cHei` cells.
         */
        std::vector<int64_t> int_grid_csv;
        /**
         * Reference the Layer definition UID
         */
        int64_t layer_def_uid;
        /**
         * Reference to the UID of the level containing this layer instance
         */
        int64_t level_id;
        /**
         * An Array containing the UIDs of optional rules that were enabled in this specific layer
         * instance.
         */
        std::vector<int64_t> optional_rules;
        /**
         * This layer can use another tileset by overriding the tileset UID here.
         */
        std::shared_ptr<int64_t> override_tileset_uid;
        /**
         * X offset in pixels to render this layer, usually 0 (IMPORTANT: this should be added to
         * the `LayerDef` optional offset, see `__pxTotalOffsetX`)
         */
        int64_t px_offset_x;
        /**
         * Y offset in pixels to render this layer, usually 0 (IMPORTANT: this should be added to
         * the `LayerDef` optional offset, see `__pxTotalOffsetY`)
         */
        int64_t px_offset_y;
        /**
         * Random seed used for Auto-Layers rendering
         */
        int64_t seed;
        /**
         * Layer instance visibility
         */
        bool visible;
    };

    enum class BgPos : int { CONTAIN, COVER, COVER_DIRTY, UNSCALED };

    /**
     * Nearby level info
     */
    struct NeighbourLevel {
        /**
         * A single lowercase character tipping on the level location (`n`orth, `s`outh, `w`est,
         * `e`ast).
         */
        std::string dir;
        int64_t level_uid;
    };

    /**
     * This section contains all the level data. It can be found in 2 distinct forms, depending
     * on Project current settings:  - If "*Separate level files*" is **disabled** (default):
     * full level data is *embedded* inside the main Project JSON file, - If "*Separate level
     * files*" is **enabled**: level data is stored in *separate* standalone `.ldtkl` files (one
     * per level). In this case, the main Project JSON file will still contain most level data,
     * except heavy sections, like the `layerInstances` array (which will be null). The
     * `externalRelPath` string points to the `ldtkl` file.  A `ldtkl` file is just a JSON file
     * containing exactly what is described below.
     */
    struct Level {
        /**
         * Background color of the level (same as `bgColor`, except the default value is
         * automatically used here if its value is `null`)
         */
        std::string bg_color;
        /**
         * Position informations of the background image, if there is one.
         */
        std::shared_ptr<LDTk::LevelBackgroundPosition> bg_pos;
        /**
         * An array listing all other levels touching this one on the world map. In "linear" world
         * layouts, this array is populated with previous/next levels in array, and `dir` depends on
         * the linear horizontal/vertical layout.
         */
        std::vector<LDTk::NeighbourLevel> neighbours;
        /**
         * Background color of the level. If `null`, the project `defaultLevelBgColor` should be
         * used.
         */
        std::shared_ptr<std::string> level_bg_color;
        /**
         * Background image X pivot (0-1)
         */
        double bg_pivot_x;
        /**
         * Background image Y pivot (0-1)
         */
        double bg_pivot_y;
        /**
         * An enum defining the way the background image (if any) is positioned on the level. See
         * `__bgPos` for resulting position info. Possible values: &lt;`null`&gt;, `Unscaled`,
         * `Contain`, `Cover`, `CoverDirty`
         */
        std::shared_ptr<LDTk::BgPos> level_bg_pos;
        /**
         * The *optional* relative path to the level background image.
         */
        std::shared_ptr<std::string> bg_rel_path;
        /**
         * This value is not null if the project option "*Save levels separately*" is enabled. In
         * this case, this **relative** path points to the level Json file.
         */
        std::shared_ptr<std::string> external_rel_path;
        /**
         * An array containing this level custom field values.
         */
        std::vector<LDTk::FieldInstance> field_instances;
        /**
         * Unique String identifier
         */
        std::string identifier;
        /**
         * An array containing all Layer instances. **IMPORTANT**: if the project option "*Save
         * levels separately*" is enabled, this field will be `null`.<br/>  This array is **sorted
         * in display order**: the 1st layer is the top-most and the last is behind.
         */
        std::shared_ptr<std::vector<LDTk::LayerInstance>> layer_instances;
        /**
         * Height of the level in pixels
         */
        int64_t px_hei;
        /**
         * Width of the level in pixels
         */
        int64_t px_wid;
        /**
         * Unique Int identifier
         */
        int64_t uid;
        /**
         * If TRUE, the level identifier will always automatically use the naming pattern as defined
         * in `Project.levelNamePattern`. Becomes FALSE if the identifier is manually modified by
         * user.
         */
        bool use_auto_identifier;
        /**
         * World X coordinate in pixels
         */
        int64_t world_x;
        /**
         * World Y coordinate in pixels
         */
        int64_t world_y;
    };

    /**
     * An enum that describes how levels are organized in this project (ie. linearly or in a 2D
     * space). Possible values: `Free`, `GridVania`, `LinearHorizontal`, `LinearVertical`
     */
    enum class WorldLayout : int { FREE, GRID_VANIA, LINEAR_HORIZONTAL, LINEAR_VERTICAL };

    /**
     * This file is a JSON schema of files created by LDtk level editor (https://ldtk.io).
     *
     * This is the root of any Project JSON file. It contains:  - the project settings, - an
     * array of levels, - a group of definitions (that can probably be safely ignored for most
     * users).
     */
    struct File {
        /**
         * Number of backup files to keep, if the `backupOnSave` is TRUE
         */
        int64_t backup_limit;
        /**
         * If TRUE, an extra copy of the project will be created in a sub folder, when saving.
         */
        bool backup_on_save;
        /**
         * Project background color
         */
        std::string bg_color;
        /**
         * Default grid size for new layers
         */
        int64_t default_grid_size;
        /**
         * Default background color of levels
         */
        std::string default_level_bg_color;
        /**
         * Default new level height
         */
        int64_t default_level_height;
        /**
         * Default new level width
         */
        int64_t default_level_width;
        /**
         * Default X pivot (0 to 1) for new entities
         */
        double default_pivot_x;
        /**
         * Default Y pivot (0 to 1) for new entities
         */
        double default_pivot_y;
        /**
         * A structure containing all the definitions of this project
         */
        LDTk::Definitions defs;
        /**
         * **WARNING**: this deprecated value is no longer exported since version 0.9.3  Replaced
         * by: `imageExportMode`
         */
        std::shared_ptr<bool> export_png;
        /**
         * If TRUE, a Tiled compatible file will also be generated along with the LDtk JSON file
         * (default is FALSE)
         */
        bool export_tiled;
        /**
         * If TRUE, one file will be saved for the project (incl. all its definitions) and one file
         * in a sub-folder for each level.
         */
        bool external_levels;
        /**
         * An array containing various advanced flags (ie. options or other states). Possible
         * values: `DiscardPreCsvIntGrid`, `IgnoreBackupSuggest`
         */
        std::vector<LDTk::Flag> flags;
        /**
         * "Image export" option when saving project. Possible values: `None`, `OneImagePerLayer`,
         * `OneImagePerLevel`
         */
        LDTk::ImageExportMode image_export_mode;
        /**
         * File format version
         */
        std::string json_version;
        /**
         * The default naming convention for level identifiers.
         */
        std::string level_name_pattern;
        /**
         * All levels. The order of this array is only relevant in `LinearHorizontal` and
         * `linearVertical` world layouts (see `worldLayout` value). Otherwise, you should refer to
         * the `worldX`,`worldY` coordinates of each Level.
         */
        std::vector<LDTk::Level> levels;
        /**
         * If TRUE, the Json is partially minified (no indentation, nor line breaks, default is
         * FALSE)
         */
        bool minify_json;
        /**
         * Next Unique integer ID available
         */
        int64_t next_uid;
        /**
         * File naming pattern for exported PNGs
         */
        std::shared_ptr<std::string> png_file_pattern;
        /**
         * Height of the world grid in pixels.
         */
        int64_t world_grid_height;
        /**
         * Width of the world grid in pixels.
         */
        int64_t world_grid_width;
        /**
         * An enum that describes how levels are organized in this project (ie. linearly or in a 2D
         * space). Possible values: `Free`, `GridVania`, `LinearHorizontal`, `LinearVertical`
         */
        LDTk::WorldLayout world_layout;
    };
}

namespace nlohmann {
namespace detail {
    void from_json(const json & j, LDTk::FieldDefinition & x);
    void to_json(json & j, const LDTk::FieldDefinition & x);

    void from_json(const json & j, LDTk::EntityDefinition & x);
    void to_json(json & j, const LDTk::EntityDefinition & x);

    void from_json(const json & j, LDTk::EnumValueDefinition & x);
    void to_json(json & j, const LDTk::EnumValueDefinition & x);

    void from_json(const json & j, LDTk::EnumDefinition & x);
    void to_json(json & j, const LDTk::EnumDefinition & x);

    void from_json(const json & j, LDTk::AutoLayerRuleDefinition & x);
    void to_json(json & j, const LDTk::AutoLayerRuleDefinition & x);

    void from_json(const json & j, LDTk::AutoLayerRuleGroup & x);
    void to_json(json & j, const LDTk::AutoLayerRuleGroup & x);

    void from_json(const json & j, LDTk::IntGridValueDefinition & x);
    void to_json(json & j, const LDTk::IntGridValueDefinition & x);

    void from_json(const json & j, LDTk::LayerDefinition & x);
    void to_json(json & j, const LDTk::LayerDefinition & x);

    void from_json(const json & j, LDTk::TilesetDefinition & x);
    void to_json(json & j, const LDTk::TilesetDefinition & x);

    void from_json(const json & j, LDTk::Definitions & x);
    void to_json(json & j, const LDTk::Definitions & x);

    void from_json(const json & j, LDTk::LevelBackgroundPosition & x);
    void to_json(json & j, const LDTk::LevelBackgroundPosition & x);

    void from_json(const json & j, LDTk::FieldInstance & x);
    void to_json(json & j, const LDTk::FieldInstance & x);

    void from_json(const json & j, LDTk::TileInstance & x);
    void to_json(json & j, const LDTk::TileInstance & x);

    void from_json(const json & j, LDTk::EntityInstanceTile & x);
    void to_json(json & j, const LDTk::EntityInstanceTile & x);

    void from_json(const json & j, LDTk::EntityInstance & x);
    void to_json(json & j, const LDTk::EntityInstance & x);

    void from_json(const json & j, LDTk::IntGridValueInstance & x);
    void to_json(json & j, const LDTk::IntGridValueInstance & x);

    void from_json(const json & j, LDTk::LayerInstance & x);
    void to_json(json & j, const LDTk::LayerInstance & x);

    void from_json(const json & j, LDTk::NeighbourLevel & x);
    void to_json(json & j, const LDTk::NeighbourLevel & x);

    void from_json(const json & j, LDTk::Level & x);
    void to_json(json & j, const LDTk::Level & x);

    void from_json(const json & j, LDTk::File & x);
    void to_json(json & j, const LDTk::File & x);

    void from_json(const json & j, LDTk::EditorDisplayMode & x);
    void to_json(json & j, const LDTk::EditorDisplayMode & x);

    void from_json(const json & j, LDTk::EditorDisplayPos & x);
    void to_json(json & j, const LDTk::EditorDisplayPos & x);

    void from_json(const json & j, LDTk::TextLanguageMode & x);
    void to_json(json & j, const LDTk::TextLanguageMode & x);

    void from_json(const json & j, LDTk::LimitBehavior & x);
    void to_json(json & j, const LDTk::LimitBehavior & x);

    void from_json(const json & j, LDTk::LimitScope & x);
    void to_json(json & j, const LDTk::LimitScope & x);

    void from_json(const json & j, LDTk::RenderMode & x);
    void to_json(json & j, const LDTk::RenderMode & x);

    void from_json(const json & j, LDTk::TileRenderMode & x);
    void to_json(json & j, const LDTk::TileRenderMode & x);

    void from_json(const json & j, LDTk::Checker & x);
    void to_json(json & j, const LDTk::Checker & x);

    void from_json(const json & j, LDTk::TileMode & x);
    void to_json(json & j, const LDTk::TileMode & x);

    void from_json(const json & j, LDTk::Type & x);
    void to_json(json & j, const LDTk::Type & x);

    void from_json(const json & j, LDTk::Flag & x);
    void to_json(json & j, const LDTk::Flag & x);

    void from_json(const json & j, LDTk::ImageExportMode & x);
    void to_json(json & j, const LDTk::ImageExportMode & x);

    void from_json(const json & j, LDTk::BgPos & x);
    void to_json(json & j, const LDTk::BgPos & x);

    void from_json(const json & j, LDTk::WorldLayout & x);
    void to_json(json & j, const LDTk::WorldLayout & x);

    inline void from_json(const json & j, LDTk::FieldDefinition& x) {
        x.type = j.at("__type").get<std::string>();
        x.accept_file_types = LDTk::get_optional<std::vector<std::string>>(j, "acceptFileTypes");
        x.array_max_length = LDTk::get_optional<int64_t>(j, "arrayMaxLength");
        x.array_min_length = LDTk::get_optional<int64_t>(j, "arrayMinLength");
        x.can_be_null = j.at("canBeNull").get<bool>();
        x.default_override = LDTk::get_untyped(j, "defaultOverride");
        x.editor_always_show = j.at("editorAlwaysShow").get<bool>();
        x.editor_cut_long_values = j.at("editorCutLongValues").get<bool>();
        x.editor_display_mode = j.at("editorDisplayMode").get<LDTk::EditorDisplayMode>();
        x.editor_display_pos = j.at("editorDisplayPos").get<LDTk::EditorDisplayPos>();
        x.identifier = j.at("identifier").get<std::string>();
        x.is_array = j.at("isArray").get<bool>();
        x.max = LDTk::get_optional<double>(j, "max");
        x.min = LDTk::get_optional<double>(j, "min");
        x.regex = LDTk::get_optional<std::string>(j, "regex");
        x.text_language_mode = LDTk::get_optional<LDTk::TextLanguageMode>(j, "textLanguageMode");
        x.field_definition_type = LDTk::get_untyped(j, "type");
        x.uid = j.at("uid").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::FieldDefinition & x) {
        j = json::object();
        j["__type"] = x.type;
        j["acceptFileTypes"] = x.accept_file_types;
        j["arrayMaxLength"] = x.array_max_length;
        j["arrayMinLength"] = x.array_min_length;
        j["canBeNull"] = x.can_be_null;
        j["defaultOverride"] = x.default_override;
        j["editorAlwaysShow"] = x.editor_always_show;
        j["editorCutLongValues"] = x.editor_cut_long_values;
        j["editorDisplayMode"] = x.editor_display_mode;
        j["editorDisplayPos"] = x.editor_display_pos;
        j["identifier"] = x.identifier;
        j["isArray"] = x.is_array;
        j["max"] = x.max;
        j["min"] = x.min;
        j["regex"] = x.regex;
        j["textLanguageMode"] = x.text_language_mode;
        j["type"] = x.field_definition_type;
        j["uid"] = x.uid;
    }

    inline void from_json(const json & j, LDTk::EntityDefinition& x) {
        x.color = j.at("color").get<std::string>();
        x.field_defs = j.at("fieldDefs").get<std::vector<LDTk::FieldDefinition>>();
        x.fill_opacity = j.at("fillOpacity").get<double>();
        x.height = j.at("height").get<int64_t>();
        x.hollow = j.at("hollow").get<bool>();
        x.identifier = j.at("identifier").get<std::string>();
        x.keep_aspect_ratio = j.at("keepAspectRatio").get<bool>();
        x.limit_behavior = j.at("limitBehavior").get<LDTk::LimitBehavior>();
        x.limit_scope = j.at("limitScope").get<LDTk::LimitScope>();
        x.line_opacity = j.at("lineOpacity").get<double>();
        x.max_count = j.at("maxCount").get<int64_t>();
        x.pivot_x = j.at("pivotX").get<double>();
        x.pivot_y = j.at("pivotY").get<double>();
        x.render_mode = j.at("renderMode").get<LDTk::RenderMode>();
        x.resizable_x = j.at("resizableX").get<bool>();
        x.resizable_y = j.at("resizableY").get<bool>();
        x.show_name = j.at("showName").get<bool>();
        x.tags = j.at("tags").get<std::vector<std::string>>();
        x.tile_id = LDTk::get_optional<int64_t>(j, "tileId");
        x.tile_render_mode = j.at("tileRenderMode").get<LDTk::TileRenderMode>();
        x.tileset_id = LDTk::get_optional<int64_t>(j, "tilesetId");
        x.uid = j.at("uid").get<int64_t>();
        x.width = j.at("width").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::EntityDefinition & x) {
        j = json::object();
        j["color"] = x.color;
        j["fieldDefs"] = x.field_defs;
        j["fillOpacity"] = x.fill_opacity;
        j["height"] = x.height;
        j["hollow"] = x.hollow;
        j["identifier"] = x.identifier;
        j["keepAspectRatio"] = x.keep_aspect_ratio;
        j["limitBehavior"] = x.limit_behavior;
        j["limitScope"] = x.limit_scope;
        j["lineOpacity"] = x.line_opacity;
        j["maxCount"] = x.max_count;
        j["pivotX"] = x.pivot_x;
        j["pivotY"] = x.pivot_y;
        j["renderMode"] = x.render_mode;
        j["resizableX"] = x.resizable_x;
        j["resizableY"] = x.resizable_y;
        j["showName"] = x.show_name;
        j["tags"] = x.tags;
        j["tileId"] = x.tile_id;
        j["tileRenderMode"] = x.tile_render_mode;
        j["tilesetId"] = x.tileset_id;
        j["uid"] = x.uid;
        j["width"] = x.width;
    }

    inline void from_json(const json & j, LDTk::EnumValueDefinition& x) {
        x.tile_src_rect = LDTk::get_optional<std::vector<int64_t>>(j, "__tileSrcRect");
        x.color = j.at("color").get<int64_t>();
        x.id = j.at("id").get<std::string>();
        x.tile_id = LDTk::get_optional<int64_t>(j, "tileId");
    }

    inline void to_json(json & j, const LDTk::EnumValueDefinition & x) {
        j = json::object();
        j["__tileSrcRect"] = x.tile_src_rect;
        j["color"] = x.color;
        j["id"] = x.id;
        j["tileId"] = x.tile_id;
    }

    inline void from_json(const json & j, LDTk::EnumDefinition& x) {
        x.external_file_checksum = LDTk::get_optional<std::string>(j, "externalFileChecksum");
        x.external_rel_path = LDTk::get_optional<std::string>(j, "externalRelPath");
        x.icon_tileset_uid = LDTk::get_optional<int64_t>(j, "iconTilesetUid");
        x.identifier = j.at("identifier").get<std::string>();
        x.uid = j.at("uid").get<int64_t>();
        x.values = j.at("values").get<std::vector<LDTk::EnumValueDefinition>>();
    }

    inline void to_json(json & j, const LDTk::EnumDefinition & x) {
        j = json::object();
        j["externalFileChecksum"] = x.external_file_checksum;
        j["externalRelPath"] = x.external_rel_path;
        j["iconTilesetUid"] = x.icon_tileset_uid;
        j["identifier"] = x.identifier;
        j["uid"] = x.uid;
        j["values"] = x.values;
    }

    inline void from_json(const json & j, LDTk::AutoLayerRuleDefinition& x) {
        x.active = j.at("active").get<bool>();
        x.break_on_match = j.at("breakOnMatch").get<bool>();
        x.chance = j.at("chance").get<double>();
        x.checker = j.at("checker").get<LDTk::Checker>();
        x.flip_x = j.at("flipX").get<bool>();
        x.flip_y = j.at("flipY").get<bool>();
        x.out_of_bounds_value = LDTk::get_optional<int64_t>(j, "outOfBoundsValue");
        x.pattern = j.at("pattern").get<std::vector<int64_t>>();
        x.perlin_active = j.at("perlinActive").get<bool>();
        x.perlin_octaves = j.at("perlinOctaves").get<double>();
        x.perlin_scale = j.at("perlinScale").get<double>();
        x.perlin_seed = j.at("perlinSeed").get<double>();
        x.pivot_x = j.at("pivotX").get<double>();
        x.pivot_y = j.at("pivotY").get<double>();
        x.size = j.at("size").get<int64_t>();
        x.tile_ids = j.at("tileIds").get<std::vector<int64_t>>();
        x.tile_mode = j.at("tileMode").get<LDTk::TileMode>();
        x.uid = j.at("uid").get<int64_t>();
        x.x_modulo = j.at("xModulo").get<int64_t>();
        x.y_modulo = j.at("yModulo").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::AutoLayerRuleDefinition & x) {
        j = json::object();
        j["active"] = x.active;
        j["breakOnMatch"] = x.break_on_match;
        j["chance"] = x.chance;
        j["checker"] = x.checker;
        j["flipX"] = x.flip_x;
        j["flipY"] = x.flip_y;
        j["outOfBoundsValue"] = x.out_of_bounds_value;
        j["pattern"] = x.pattern;
        j["perlinActive"] = x.perlin_active;
        j["perlinOctaves"] = x.perlin_octaves;
        j["perlinScale"] = x.perlin_scale;
        j["perlinSeed"] = x.perlin_seed;
        j["pivotX"] = x.pivot_x;
        j["pivotY"] = x.pivot_y;
        j["size"] = x.size;
        j["tileIds"] = x.tile_ids;
        j["tileMode"] = x.tile_mode;
        j["uid"] = x.uid;
        j["xModulo"] = x.x_modulo;
        j["yModulo"] = x.y_modulo;
    }

    inline void from_json(const json & j, LDTk::AutoLayerRuleGroup& x) {
        x.active = j.at("active").get<bool>();
        x.collapsed = j.at("collapsed").get<bool>();
        x.is_optional = j.at("isOptional").get<bool>();
        x.name = j.at("name").get<std::string>();
        x.rules = j.at("rules").get<std::vector<LDTk::AutoLayerRuleDefinition>>();
        x.uid = j.at("uid").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::AutoLayerRuleGroup & x) {
        j = json::object();
        j["active"] = x.active;
        j["collapsed"] = x.collapsed;
        j["isOptional"] = x.is_optional;
        j["name"] = x.name;
        j["rules"] = x.rules;
        j["uid"] = x.uid;
    }

    inline void from_json(const json & j, LDTk::IntGridValueDefinition& x) {
        x.color = j.at("color").get<std::string>();
        x.identifier = LDTk::get_optional<std::string>(j, "identifier");
        x.value = j.at("value").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::IntGridValueDefinition & x) {
        j = json::object();
        j["color"] = x.color;
        j["identifier"] = x.identifier;
        j["value"] = x.value;
    }

    inline void from_json(const json & j, LDTk::LayerDefinition& x) {
        x.type = j.at("__type").get<std::string>();
        x.auto_rule_groups = j.at("autoRuleGroups").get<std::vector<LDTk::AutoLayerRuleGroup>>();
        x.auto_source_layer_def_uid = LDTk::get_optional<int64_t>(j, "autoSourceLayerDefUid");
        x.auto_tileset_def_uid = LDTk::get_optional<int64_t>(j, "autoTilesetDefUid");
        x.display_opacity = j.at("displayOpacity").get<double>();
        x.excluded_tags = j.at("excludedTags").get<std::vector<std::string>>();
        x.grid_size = j.at("gridSize").get<int64_t>();
        x.identifier = j.at("identifier").get<std::string>();
        x.int_grid_values = j.at("intGridValues").get<std::vector<LDTk::IntGridValueDefinition>>();
        x.px_offset_x = j.at("pxOffsetX").get<int64_t>();
        x.px_offset_y = j.at("pxOffsetY").get<int64_t>();
        x.required_tags = j.at("requiredTags").get<std::vector<std::string>>();
        x.tile_pivot_x = j.at("tilePivotX").get<double>();
        x.tile_pivot_y = j.at("tilePivotY").get<double>();
        x.tileset_def_uid = LDTk::get_optional<int64_t>(j, "tilesetDefUid");
        x.layer_definition_type = j.at("type").get<LDTk::Type>();
        x.uid = j.at("uid").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::LayerDefinition & x) {
        j = json::object();
        j["__type"] = x.type;
        j["autoRuleGroups"] = x.auto_rule_groups;
        j["autoSourceLayerDefUid"] = x.auto_source_layer_def_uid;
        j["autoTilesetDefUid"] = x.auto_tileset_def_uid;
        j["displayOpacity"] = x.display_opacity;
        j["excludedTags"] = x.excluded_tags;
        j["gridSize"] = x.grid_size;
        j["identifier"] = x.identifier;
        j["intGridValues"] = x.int_grid_values;
        j["pxOffsetX"] = x.px_offset_x;
        j["pxOffsetY"] = x.px_offset_y;
        j["requiredTags"] = x.required_tags;
        j["tilePivotX"] = x.tile_pivot_x;
        j["tilePivotY"] = x.tile_pivot_y;
        j["tilesetDefUid"] = x.tileset_def_uid;
        j["type"] = x.layer_definition_type;
        j["uid"] = x.uid;
    }

    inline void from_json(const json & j, LDTk::TilesetDefinition& x) {
        x.c_hei = j.at("__cHei").get<int64_t>();
        x.c_wid = j.at("__cWid").get<int64_t>();
        x.cached_pixel_data = LDTk::get_optional<std::map<std::string, json>>(j, "cachedPixelData");
        x.custom_data = j.at("customData").get<std::vector<std::map<std::string, json>>>();
        x.enum_tags = j.at("enumTags").get<std::vector<std::map<std::string, json>>>();
        x.identifier = j.at("identifier").get<std::string>();
        x.padding = j.at("padding").get<int64_t>();
        x.px_hei = j.at("pxHei").get<int64_t>();
        x.px_wid = j.at("pxWid").get<int64_t>();
        x.rel_path = j.at("relPath").get<std::string>();
        x.saved_selections = j.at("savedSelections").get<std::vector<std::map<std::string, json>>>();
        x.spacing = j.at("spacing").get<int64_t>();
        x.tags_source_enum_uid = LDTk::get_optional<int64_t>(j, "tagsSourceEnumUid");
        x.tile_grid_size = j.at("tileGridSize").get<int64_t>();
        x.uid = j.at("uid").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::TilesetDefinition & x) {
        j = json::object();
        j["__cHei"] = x.c_hei;
        j["__cWid"] = x.c_wid;
        j["cachedPixelData"] = x.cached_pixel_data;
        j["customData"] = x.custom_data;
        j["enumTags"] = x.enum_tags;
        j["identifier"] = x.identifier;
        j["padding"] = x.padding;
        j["pxHei"] = x.px_hei;
        j["pxWid"] = x.px_wid;
        j["relPath"] = x.rel_path;
        j["savedSelections"] = x.saved_selections;
        j["spacing"] = x.spacing;
        j["tagsSourceEnumUid"] = x.tags_source_enum_uid;
        j["tileGridSize"] = x.tile_grid_size;
        j["uid"] = x.uid;
    }

    inline void from_json(const json & j, LDTk::Definitions& x) {
        x.entities = j.at("entities").get<std::vector<LDTk::EntityDefinition>>();
        x.enums = j.at("enums").get<std::vector<LDTk::EnumDefinition>>();
        x.external_enums = j.at("externalEnums").get<std::vector<LDTk::EnumDefinition>>();
        x.layers = j.at("layers").get<std::vector<LDTk::LayerDefinition>>();
        x.level_fields = j.at("levelFields").get<std::vector<LDTk::FieldDefinition>>();
        x.tilesets = j.at("tilesets").get<std::vector<LDTk::TilesetDefinition>>();
    }

    inline void to_json(json & j, const LDTk::Definitions & x) {
        j = json::object();
        j["entities"] = x.entities;
        j["enums"] = x.enums;
        j["externalEnums"] = x.external_enums;
        j["layers"] = x.layers;
        j["levelFields"] = x.level_fields;
        j["tilesets"] = x.tilesets;
    }

    inline void from_json(const json & j, LDTk::LevelBackgroundPosition& x) {
        x.crop_rect = j.at("cropRect").get<std::vector<double>>();
        x.scale = j.at("scale").get<std::vector<double>>();
        x.top_left_px = j.at("topLeftPx").get<std::vector<int64_t>>();
    }

    inline void to_json(json & j, const LDTk::LevelBackgroundPosition & x) {
        j = json::object();
        j["cropRect"] = x.crop_rect;
        j["scale"] = x.scale;
        j["topLeftPx"] = x.top_left_px;
    }

    inline void from_json(const json & j, LDTk::FieldInstance& x) {
        x.identifier = j.at("__identifier").get<std::string>();
        x.type = j.at("__type").get<std::string>();
        x.value = LDTk::get_untyped(j, "__value");
        x.def_uid = j.at("defUid").get<int64_t>();
        x.real_editor_values = j.at("realEditorValues").get<std::vector<json>>();
    }

    inline void to_json(json & j, const LDTk::FieldInstance & x) {
        j = json::object();
        j["__identifier"] = x.identifier;
        j["__type"] = x.type;
        j["__value"] = x.value;
        j["defUid"] = x.def_uid;
        j["realEditorValues"] = x.real_editor_values;
    }

    inline void from_json(const json & j, LDTk::TileInstance& x) {
        x.d = j.at("d").get<std::vector<int64_t>>();
        x.f = j.at("f").get<int64_t>();
        x.px = j.at("px").get<std::vector<int64_t>>();
        x.src = j.at("src").get<std::vector<int64_t>>();
        x.t = j.at("t").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::TileInstance & x) {
        j = json::object();
        j["d"] = x.d;
        j["f"] = x.f;
        j["px"] = x.px;
        j["src"] = x.src;
        j["t"] = x.t;
    }

    inline void from_json(const json & j, LDTk::EntityInstanceTile& x) {
        x.src_rect = j.at("srcRect").get<std::vector<int64_t>>();
        x.tileset_uid = j.at("tilesetUid").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::EntityInstanceTile & x) {
        j = json::object();
        j["srcRect"] = x.src_rect;
        j["tilesetUid"] = x.tileset_uid;
    }

    inline void from_json(const json & j, LDTk::EntityInstance& x) {
        x.grid = j.at("__grid").get<std::vector<int64_t>>();
        x.identifier = j.at("__identifier").get<std::string>();
        x.pivot = j.at("__pivot").get<std::vector<double>>();
        x.tile = LDTk::get_optional<LDTk::EntityInstanceTile>(j, "__tile");
        x.def_uid = j.at("defUid").get<int64_t>();
        x.field_instances = j.at("fieldInstances").get<std::vector<LDTk::FieldInstance>>();
        x.height = j.at("height").get<int64_t>();
        x.px = j.at("px").get<std::vector<int64_t>>();
        x.width = j.at("width").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::EntityInstance & x) {
        j = json::object();
        j["__grid"] = x.grid;
        j["__identifier"] = x.identifier;
        j["__pivot"] = x.pivot;
        j["__tile"] = x.tile;
        j["defUid"] = x.def_uid;
        j["fieldInstances"] = x.field_instances;
        j["height"] = x.height;
        j["px"] = x.px;
        j["width"] = x.width;
    }

    inline void from_json(const json & j, LDTk::IntGridValueInstance& x) {
        x.coord_id = j.at("coordId").get<int64_t>();
        x.v = j.at("v").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::IntGridValueInstance & x) {
        j = json::object();
        j["coordId"] = x.coord_id;
        j["v"] = x.v;
    }

    inline void from_json(const json & j, LDTk::LayerInstance& x) {
        x.c_hei = j.at("__cHei").get<int64_t>();
        x.c_wid = j.at("__cWid").get<int64_t>();
        x.grid_size = j.at("__gridSize").get<int64_t>();
        x.identifier = j.at("__identifier").get<std::string>();
        x.opacity = j.at("__opacity").get<double>();
        x.px_total_offset_x = j.at("__pxTotalOffsetX").get<int64_t>();
        x.px_total_offset_y = j.at("__pxTotalOffsetY").get<int64_t>();
        x.tileset_def_uid = LDTk::get_optional<int64_t>(j, "__tilesetDefUid");
        x.tileset_rel_path = LDTk::get_optional<std::string>(j, "__tilesetRelPath");
        x.type = j.at("__type").get<std::string>();
        x.auto_layer_tiles = j.at("autoLayerTiles").get<std::vector<LDTk::TileInstance>>();
        x.entity_instances = j.at("entityInstances").get<std::vector<LDTk::EntityInstance>>();
        x.grid_tiles = j.at("gridTiles").get<std::vector<LDTk::TileInstance>>();
        x.int_grid = LDTk::get_optional<std::vector<LDTk::IntGridValueInstance>>(j, "intGrid");
        x.int_grid_csv = j.at("intGridCsv").get<std::vector<int64_t>>();
        x.layer_def_uid = j.at("layerDefUid").get<int64_t>();
        x.level_id = j.at("levelId").get<int64_t>();
        x.optional_rules = j.at("optionalRules").get<std::vector<int64_t>>();
        x.override_tileset_uid = LDTk::get_optional<int64_t>(j, "overrideTilesetUid");
        x.px_offset_x = j.at("pxOffsetX").get<int64_t>();
        x.px_offset_y = j.at("pxOffsetY").get<int64_t>();
        x.seed = j.at("seed").get<int64_t>();
        x.visible = j.at("visible").get<bool>();
    }

    inline void to_json(json & j, const LDTk::LayerInstance & x) {
        j = json::object();
        j["__cHei"] = x.c_hei;
        j["__cWid"] = x.c_wid;
        j["__gridSize"] = x.grid_size;
        j["__identifier"] = x.identifier;
        j["__opacity"] = x.opacity;
        j["__pxTotalOffsetX"] = x.px_total_offset_x;
        j["__pxTotalOffsetY"] = x.px_total_offset_y;
        j["__tilesetDefUid"] = x.tileset_def_uid;
        j["__tilesetRelPath"] = x.tileset_rel_path;
        j["__type"] = x.type;
        j["autoLayerTiles"] = x.auto_layer_tiles;
        j["entityInstances"] = x.entity_instances;
        j["gridTiles"] = x.grid_tiles;
        j["intGrid"] = x.int_grid;
        j["intGridCsv"] = x.int_grid_csv;
        j["layerDefUid"] = x.layer_def_uid;
        j["levelId"] = x.level_id;
        j["optionalRules"] = x.optional_rules;
        j["overrideTilesetUid"] = x.override_tileset_uid;
        j["pxOffsetX"] = x.px_offset_x;
        j["pxOffsetY"] = x.px_offset_y;
        j["seed"] = x.seed;
        j["visible"] = x.visible;
    }

    inline void from_json(const json & j, LDTk::NeighbourLevel& x) {
        x.dir = j.at("dir").get<std::string>();
        x.level_uid = j.at("levelUid").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::NeighbourLevel & x) {
        j = json::object();
        j["dir"] = x.dir;
        j["levelUid"] = x.level_uid;
    }

    inline void from_json(const json & j, LDTk::Level& x) {
        x.bg_color = j.at("__bgColor").get<std::string>();
        x.bg_pos = LDTk::get_optional<LDTk::LevelBackgroundPosition>(j, "__bgPos");
        x.neighbours = j.at("__neighbours").get<std::vector<LDTk::NeighbourLevel>>();
        x.level_bg_color = LDTk::get_optional<std::string>(j, "bgColor");
        x.bg_pivot_x = j.at("bgPivotX").get<double>();
        x.bg_pivot_y = j.at("bgPivotY").get<double>();
        x.level_bg_pos = LDTk::get_optional<LDTk::BgPos>(j, "bgPos");
        x.bg_rel_path = LDTk::get_optional<std::string>(j, "bgRelPath");
        x.external_rel_path = LDTk::get_optional<std::string>(j, "externalRelPath");
        x.field_instances = j.at("fieldInstances").get<std::vector<LDTk::FieldInstance>>();
        x.identifier = j.at("identifier").get<std::string>();
        x.layer_instances = LDTk::get_optional<std::vector<LDTk::LayerInstance>>(j, "layerInstances");
        x.px_hei = j.at("pxHei").get<int64_t>();
        x.px_wid = j.at("pxWid").get<int64_t>();
        x.uid = j.at("uid").get<int64_t>();
        x.use_auto_identifier = j.at("useAutoIdentifier").get<bool>();
        x.world_x = j.at("worldX").get<int64_t>();
        x.world_y = j.at("worldY").get<int64_t>();
    }

    inline void to_json(json & j, const LDTk::Level & x) {
        j = json::object();
        j["__bgColor"] = x.bg_color;
        j["__bgPos"] = x.bg_pos;
        j["__neighbours"] = x.neighbours;
        j["bgColor"] = x.level_bg_color;
        j["bgPivotX"] = x.bg_pivot_x;
        j["bgPivotY"] = x.bg_pivot_y;
        j["bgPos"] = x.level_bg_pos;
        j["bgRelPath"] = x.bg_rel_path;
        j["externalRelPath"] = x.external_rel_path;
        j["fieldInstances"] = x.field_instances;
        j["identifier"] = x.identifier;
        j["layerInstances"] = x.layer_instances;
        j["pxHei"] = x.px_hei;
        j["pxWid"] = x.px_wid;
        j["uid"] = x.uid;
        j["useAutoIdentifier"] = x.use_auto_identifier;
        j["worldX"] = x.world_x;
        j["worldY"] = x.world_y;
    }

    inline void from_json(const json & j, LDTk::File& x) {
        x.backup_limit = j.at("backupLimit").get<int64_t>();
        x.backup_on_save = j.at("backupOnSave").get<bool>();
        x.bg_color = j.at("bgColor").get<std::string>();
        x.default_grid_size = j.at("defaultGridSize").get<int64_t>();
        x.default_level_bg_color = j.at("defaultLevelBgColor").get<std::string>();
        x.default_level_height = j.at("defaultLevelHeight").get<int64_t>();
        x.default_level_width = j.at("defaultLevelWidth").get<int64_t>();
        x.default_pivot_x = j.at("defaultPivotX").get<double>();
        x.default_pivot_y = j.at("defaultPivotY").get<double>();
        x.defs = j.at("defs").get<LDTk::Definitions>();
        x.export_png = LDTk::get_optional<bool>(j, "exportPng");
        x.export_tiled = j.at("exportTiled").get<bool>();
        x.external_levels = j.at("externalLevels").get<bool>();
        x.flags = j.at("flags").get<std::vector<LDTk::Flag>>();
        x.image_export_mode = j.at("imageExportMode").get<LDTk::ImageExportMode>();
        x.json_version = j.at("jsonVersion").get<std::string>();
        x.level_name_pattern = j.at("levelNamePattern").get<std::string>();
        x.levels = j.at("levels").get<std::vector<LDTk::Level>>();
        x.minify_json = j.at("minifyJson").get<bool>();
        x.next_uid = j.at("nextUid").get<int64_t>();
        x.png_file_pattern = LDTk::get_optional<std::string>(j, "pngFilePattern");
        x.world_grid_height = j.at("worldGridHeight").get<int64_t>();
        x.world_grid_width = j.at("worldGridWidth").get<int64_t>();
        x.world_layout = j.at("worldLayout").get<LDTk::WorldLayout>();
    }

    inline void to_json(json & j, const LDTk::File & x) {
        j = json::object();
        j["backupLimit"] = x.backup_limit;
        j["backupOnSave"] = x.backup_on_save;
        j["bgColor"] = x.bg_color;
        j["defaultGridSize"] = x.default_grid_size;
        j["defaultLevelBgColor"] = x.default_level_bg_color;
        j["defaultLevelHeight"] = x.default_level_height;
        j["defaultLevelWidth"] = x.default_level_width;
        j["defaultPivotX"] = x.default_pivot_x;
        j["defaultPivotY"] = x.default_pivot_y;
        j["defs"] = x.defs;
        j["exportPng"] = x.export_png;
        j["exportTiled"] = x.export_tiled;
        j["externalLevels"] = x.external_levels;
        j["flags"] = x.flags;
        j["imageExportMode"] = x.image_export_mode;
        j["jsonVersion"] = x.json_version;
        j["levelNamePattern"] = x.level_name_pattern;
        j["levels"] = x.levels;
        j["minifyJson"] = x.minify_json;
        j["nextUid"] = x.next_uid;
        j["pngFilePattern"] = x.png_file_pattern;
        j["worldGridHeight"] = x.world_grid_height;
        j["worldGridWidth"] = x.world_grid_width;
        j["worldLayout"] = x.world_layout;
    }

    inline void from_json(const json & j, LDTk::EditorDisplayMode & x) {
        if (j == "EntityTile") x = LDTk::EditorDisplayMode::ENTITY_TILE;
        else if (j == "Hidden") x = LDTk::EditorDisplayMode::HIDDEN;
        else if (j == "NameAndValue") x = LDTk::EditorDisplayMode::NAME_AND_VALUE;
        else if (j == "Points") x = LDTk::EditorDisplayMode::POINTS;
        else if (j == "PointPath") x = LDTk::EditorDisplayMode::POINT_PATH;
        else if (j == "PointPathLoop") x = LDTk::EditorDisplayMode::POINT_PATH_LOOP;
        else if (j == "PointStar") x = LDTk::EditorDisplayMode::POINT_STAR;
        else if (j == "RadiusGrid") x = LDTk::EditorDisplayMode::RADIUS_GRID;
        else if (j == "RadiusPx") x = LDTk::EditorDisplayMode::RADIUS_PX;
        else if (j == "ValueOnly") x = LDTk::EditorDisplayMode::VALUE_ONLY;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::EditorDisplayMode & x) {
        switch (x) {
            case LDTk::EditorDisplayMode::ENTITY_TILE: j = "EntityTile"; break;
            case LDTk::EditorDisplayMode::HIDDEN: j = "Hidden"; break;
            case LDTk::EditorDisplayMode::NAME_AND_VALUE: j = "NameAndValue"; break;
            case LDTk::EditorDisplayMode::POINTS: j = "Points"; break;
            case LDTk::EditorDisplayMode::POINT_PATH: j = "PointPath"; break;
            case LDTk::EditorDisplayMode::POINT_PATH_LOOP: j = "PointPathLoop"; break;
            case LDTk::EditorDisplayMode::POINT_STAR: j = "PointStar"; break;
            case LDTk::EditorDisplayMode::RADIUS_GRID: j = "RadiusGrid"; break;
            case LDTk::EditorDisplayMode::RADIUS_PX: j = "RadiusPx"; break;
            case LDTk::EditorDisplayMode::VALUE_ONLY: j = "ValueOnly"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::EditorDisplayPos & x) {
        if (j == "Above") x = LDTk::EditorDisplayPos::ABOVE;
        else if (j == "Beneath") x = LDTk::EditorDisplayPos::BENEATH;
        else if (j == "Center") x = LDTk::EditorDisplayPos::CENTER;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::EditorDisplayPos & x) {
        switch (x) {
            case LDTk::EditorDisplayPos::ABOVE: j = "Above"; break;
            case LDTk::EditorDisplayPos::BENEATH: j = "Beneath"; break;
            case LDTk::EditorDisplayPos::CENTER: j = "Center"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::TextLanguageMode & x) {
        if (j == "LangC") x = LDTk::TextLanguageMode::LANG_C;
        else if (j == "LangHaxe") x = LDTk::TextLanguageMode::LANG_HAXE;
        else if (j == "LangJS") x = LDTk::TextLanguageMode::LANG_JS;
        else if (j == "LangJson") x = LDTk::TextLanguageMode::LANG_JSON;
        else if (j == "LangLua") x = LDTk::TextLanguageMode::LANG_LUA;
        else if (j == "LangMarkdown") x = LDTk::TextLanguageMode::LANG_MARKDOWN;
        else if (j == "LangPython") x = LDTk::TextLanguageMode::LANG_PYTHON;
        else if (j == "LangRuby") x = LDTk::TextLanguageMode::LANG_RUBY;
        else if (j == "LangXml") x = LDTk::TextLanguageMode::LANG_XML;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::TextLanguageMode & x) {
        switch (x) {
            case LDTk::TextLanguageMode::LANG_C: j = "LangC"; break;
            case LDTk::TextLanguageMode::LANG_HAXE: j = "LangHaxe"; break;
            case LDTk::TextLanguageMode::LANG_JS: j = "LangJS"; break;
            case LDTk::TextLanguageMode::LANG_JSON: j = "LangJson"; break;
            case LDTk::TextLanguageMode::LANG_LUA: j = "LangLua"; break;
            case LDTk::TextLanguageMode::LANG_MARKDOWN: j = "LangMarkdown"; break;
            case LDTk::TextLanguageMode::LANG_PYTHON: j = "LangPython"; break;
            case LDTk::TextLanguageMode::LANG_RUBY: j = "LangRuby"; break;
            case LDTk::TextLanguageMode::LANG_XML: j = "LangXml"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::LimitBehavior & x) {
        if (j == "DiscardOldOnes") x = LDTk::LimitBehavior::DISCARD_OLD_ONES;
        else if (j == "MoveLastOne") x = LDTk::LimitBehavior::MOVE_LAST_ONE;
        else if (j == "PreventAdding") x = LDTk::LimitBehavior::PREVENT_ADDING;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::LimitBehavior & x) {
        switch (x) {
            case LDTk::LimitBehavior::DISCARD_OLD_ONES: j = "DiscardOldOnes"; break;
            case LDTk::LimitBehavior::MOVE_LAST_ONE: j = "MoveLastOne"; break;
            case LDTk::LimitBehavior::PREVENT_ADDING: j = "PreventAdding"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::LimitScope & x) {
        if (j == "PerLayer") x = LDTk::LimitScope::PER_LAYER;
        else if (j == "PerLevel") x = LDTk::LimitScope::PER_LEVEL;
        else if (j == "PerWorld") x = LDTk::LimitScope::PER_WORLD;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::LimitScope & x) {
        switch (x) {
            case LDTk::LimitScope::PER_LAYER: j = "PerLayer"; break;
            case LDTk::LimitScope::PER_LEVEL: j = "PerLevel"; break;
            case LDTk::LimitScope::PER_WORLD: j = "PerWorld"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::RenderMode & x) {
        if (j == "Cross") x = LDTk::RenderMode::CROSS;
        else if (j == "Ellipse") x = LDTk::RenderMode::ELLIPSE;
        else if (j == "Rectangle") x = LDTk::RenderMode::RECTANGLE;
        else if (j == "Tile") x = LDTk::RenderMode::TILE;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::RenderMode & x) {
        switch (x) {
            case LDTk::RenderMode::CROSS: j = "Cross"; break;
            case LDTk::RenderMode::ELLIPSE: j = "Ellipse"; break;
            case LDTk::RenderMode::RECTANGLE: j = "Rectangle"; break;
            case LDTk::RenderMode::TILE: j = "Tile"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::TileRenderMode & x) {
        if (j == "Cover") x = LDTk::TileRenderMode::COVER;
        else if (j == "FitInside") x = LDTk::TileRenderMode::FIT_INSIDE;
        else if (j == "Repeat") x = LDTk::TileRenderMode::REPEAT;
        else if (j == "Stretch") x = LDTk::TileRenderMode::STRETCH;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::TileRenderMode & x) {
        switch (x) {
            case LDTk::TileRenderMode::COVER: j = "Cover"; break;
            case LDTk::TileRenderMode::FIT_INSIDE: j = "FitInside"; break;
            case LDTk::TileRenderMode::REPEAT: j = "Repeat"; break;
            case LDTk::TileRenderMode::STRETCH: j = "Stretch"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::Checker & x) {
        if (j == "Horizontal") x = LDTk::Checker::HORIZONTAL;
        else if (j == "None") x = LDTk::Checker::NONE;
        else if (j == "Vertical") x = LDTk::Checker::VERTICAL;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::Checker & x) {
        switch (x) {
            case LDTk::Checker::HORIZONTAL: j = "Horizontal"; break;
            case LDTk::Checker::NONE: j = "None"; break;
            case LDTk::Checker::VERTICAL: j = "Vertical"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::TileMode & x) {
        if (j == "Single") x = LDTk::TileMode::SINGLE;
        else if (j == "Stamp") x = LDTk::TileMode::STAMP;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::TileMode & x) {
        switch (x) {
            case LDTk::TileMode::SINGLE: j = "Single"; break;
            case LDTk::TileMode::STAMP: j = "Stamp"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::Type & x) {
        if (j == "AutoLayer") x = LDTk::Type::AUTO_LAYER;
        else if (j == "Entities") x = LDTk::Type::ENTITIES;
        else if (j == "IntGrid") x = LDTk::Type::INT_GRID;
        else if (j == "Tiles") x = LDTk::Type::TILES;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::Type & x) {
        switch (x) {
            case LDTk::Type::AUTO_LAYER: j = "AutoLayer"; break;
            case LDTk::Type::ENTITIES: j = "Entities"; break;
            case LDTk::Type::INT_GRID: j = "IntGrid"; break;
            case LDTk::Type::TILES: j = "Tiles"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::Flag & x) {
        if (j == "DiscardPreCsvIntGrid") x = LDTk::Flag::DISCARD_PRE_CSV_INT_GRID;
        else if (j == "IgnoreBackupSuggest") x = LDTk::Flag::IGNORE_BACKUP_SUGGEST;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::Flag & x) {
        switch (x) {
            case LDTk::Flag::DISCARD_PRE_CSV_INT_GRID: j = "DiscardPreCsvIntGrid"; break;
            case LDTk::Flag::IGNORE_BACKUP_SUGGEST: j = "IgnoreBackupSuggest"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::ImageExportMode & x) {
        if (j == "None") x = LDTk::ImageExportMode::NONE;
        else if (j == "OneImagePerLayer") x = LDTk::ImageExportMode::ONE_IMAGE_PER_LAYER;
        else if (j == "OneImagePerLevel") x = LDTk::ImageExportMode::ONE_IMAGE_PER_LEVEL;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::ImageExportMode & x) {
        switch (x) {
            case LDTk::ImageExportMode::NONE: j = "None"; break;
            case LDTk::ImageExportMode::ONE_IMAGE_PER_LAYER: j = "OneImagePerLayer"; break;
            case LDTk::ImageExportMode::ONE_IMAGE_PER_LEVEL: j = "OneImagePerLevel"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::BgPos & x) {
        if (j == "Contain") x = LDTk::BgPos::CONTAIN;
        else if (j == "Cover") x = LDTk::BgPos::COVER;
        else if (j == "CoverDirty") x = LDTk::BgPos::COVER_DIRTY;
        else if (j == "Unscaled") x = LDTk::BgPos::UNSCALED;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::BgPos & x) {
        switch (x) {
            case LDTk::BgPos::CONTAIN: j = "Contain"; break;
            case LDTk::BgPos::COVER: j = "Cover"; break;
            case LDTk::BgPos::COVER_DIRTY: j = "CoverDirty"; break;
            case LDTk::BgPos::UNSCALED: j = "Unscaled"; break;
            default: throw "This should not happen";
        }
    }

    inline void from_json(const json & j, LDTk::WorldLayout & x) {
        if (j == "Free") x = LDTk::WorldLayout::FREE;
        else if (j == "GridVania") x = LDTk::WorldLayout::GRID_VANIA;
        else if (j == "LinearHorizontal") x = LDTk::WorldLayout::LINEAR_HORIZONTAL;
        else if (j == "LinearVertical") x = LDTk::WorldLayout::LINEAR_VERTICAL;
        else throw "Input JSON does not conform to schema";
    }

    inline void to_json(json & j, const LDTk::WorldLayout & x) {
        switch (x) {
            case LDTk::WorldLayout::FREE: j = "Free"; break;
            case LDTk::WorldLayout::GRID_VANIA: j = "GridVania"; break;
            case LDTk::WorldLayout::LINEAR_HORIZONTAL: j = "LinearHorizontal"; break;
            case LDTk::WorldLayout::LINEAR_VERTICAL: j = "LinearVertical"; break;
            default: throw "This should not happen";
        }
    }
}
}
