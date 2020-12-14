//
// YaPB - Counter-Strike Bot based on PODBot by Markus Klinge.
// Copyright © 2004-2020 YaPB Project <yapb@jeefo.net>.
//
// SPDX-License-Identifier: MIT
//

#include <yapb.h>

/*
* Gets the YaPB version string.
*
* @param output     Buffer to copy version to.
* @param len        Maximum buffer size.
*
* @noreturn
*/
cell AMX_NATIVE_CALL yb_get_bot_version (AMX *amx, cell *params) {
   return MF_SetAmxString (amx, params[1], yapb->api ()->getBotVersion (), params[2]);
}

/**
 * Checks whether bots are added on server.
 *
 * @return          True if bots are added to server, false otherwise.
 */
cell AMX_NATIVE_CALL yb_game_has_bots (AMX *amx, cell *params) {
   return yapb->api ()->isBotsInGame () ? 1 : 0;
}

/**
 * Checks if player is an YaPB bot.
 *
 * @param index     Player index.
 *
 * @return          True if players is YaPB bot, false otherwise.
 */
cell AMX_NATIVE_CALL yb_is_user_bot (AMX *amx, cell *params) {
   return yapb->api ()->isBot (params[1]);
}

/**
 * Retrieves graph node index near the location.
 *
 * @param origin    Vector of the location.
 *
 * @return          Graph node index.
 */
cell AMX_NATIVE_CALL yb_get_nearest_node (AMX *amx, cell *params) {
   auto param = MF_GetAmxAddr (amx, params[1]);
   float origin[3] {};

   for (size_t i = 0; i < 3; ++i) {
      origin[i] = static_cast <float> (param[i]);
   }
   return yapb->api ()->getNearestNode (origin);
}

/**
 * Checks if graph node index is valid.
 *
 * @param node      Graph node index.
 *
 * @return          True if graph node is valid, false otherwise.
 */
cell AMX_NATIVE_CALL yb_is_node_valid (AMX *amx, cell *params) {
   return yapb->api ()->isNodeValid (params[1]) ? 1 :0;
}

/**
 * Retrieves graph node location via it's index.
 *
 * @param node      Graph node index.
 * @param origin    Array to store graph node origin.
 *
 * @noreturn
 */
cell AMX_NATIVE_CALL yb_get_node_origin (AMX *amx, cell *params) {
   auto result = yapb->api ()->getNodeOrigin (params[1]);

   if (result) {
      auto param = MF_GetAmxAddr (amx, params[2]);

      for (size_t i = 0; i < 3; ++i) {
         param[i] = static_cast <long int> (result[i]);

      }
      return 1;
   }
   return 0;
}

/**
 * Retrieves node index bot currently standing.
 *
 * @param index     Bot index.
 *
 * @note            Can return INVALID_NODE_INDEX if bot currently dead / camping.
 *
 * @return          Graph node index.
 */
cell AMX_NATIVE_CALL yb_get_bot_node (AMX *amx, cell *params) {
   return yapb->api ()->getCurrentNodeId (params[1]);
}

/**
 * Force set bot goal index with interrupting current tasks.
 *
 * @param index     Bot index.
 * @param node      Goal node index.
 *
 * @note            Goal node following can be interrupted by internal bot tasks.
 *
 * @noreturn
 */
cell AMX_NATIVE_CALL yb_set_bot_goal_node (AMX *amx, cell *params) {
   yapb->api ()->setBotGoal (params[1], params[2]);

   return 1;
}

/**
 * Retrieves node index bot currently going to (goal).
 *
 * @param index     Bot index.
 *
 * @note            Can return INVALID_NODE_INDEX if bot currently dead / camping.
 *
 * @return          Graph node index.
 */
cell AMX_NATIVE_CALL yb_get_bot_goal (AMX *amx, cell *params) {
   return yapb->api ()->getBotGoal (params[1]);
}

/**
 * Sets the bot goal location with interrupting current tasks.
 *
 * @param index     Bot index.
 * @param origin    Vector of the location.
 *
 * @note            Goal node following can be interrupted by internal bot tasks.
 *
 * @noreturn
 */
cell AMX_NATIVE_CALL yb_set_bot_goal_origin (AMX *amx, cell *params) {
   auto param = MF_GetAmxAddr (amx, params[2]);
   float origin[3] {};

   for (size_t i = 0; i < 3; ++i) {
      origin[i] = static_cast <float> (param[i]);
   }
   yapb->api ()->setBotGoalOrigin (params[1], origin);

   return 1;
}

/**
 * Checks whether current map has graph file.
 *
 * @return          True graph is available, false otherwise.
 */
cell AMX_NATIVE_CALL yb_has_graph (AMX *amx, cell *params) {
   return yapb->api ()->hasGraph () ? 1 : 0;
}

/**
 * Retrieves graph node flags via it's index.
 *
 * @param node      Graph node index.
 *
 * @return          Graph node flags.
 */
cell AMX_NATIVE_CALL yb_get_node_flags (AMX *amx, cell *params) {
   return yapb->api ()->getNodeFlags (params[1]);;
}

AMX_NATIVE_INFO botNatives[] = {
   { "yb_get_bot_version",     yb_get_bot_version },
   { "yb_game_has_bots",       yb_game_has_bots },
   { "yb_is_user_bot",         yb_is_user_bot },
   { "yb_get_nearest_node",    yb_get_nearest_node },
   { "yb_is_node_valid",       yb_is_node_valid },
   { "yb_get_node_origin",     yb_get_node_origin },
   { "yb_get_bot_node",        yb_get_bot_node },
   { "yb_set_bot_goal_node",   yb_set_bot_goal_node },
   { "yb_get_bot_goal",        yb_get_bot_goal },
   { "yb_set_bot_goal_origin", yb_set_bot_goal_origin },
   { "yb_has_graph",           yb_has_graph },
   { "yb_get_node_flags",      yb_get_node_flags },

   { nullptr, nullptr}
};

void YaPBModule::load () {
   MF_AddNatives (botNatives);

   if (botdll_) {
      botdll_.unload ();
   }
   String ext;

   if (plat.win) {
      ext = "dll";
   }
   else if (plat.nix) {
      ext = "so";
   }
   else if (plat.osx) {
      ext = "dylib";
   }
   StringRef botPath = strings.format ("%s/addons/yapb/bin/yapb.%s", MF_GetModname (), ext);
   StringRef exportFunction = "GetBotAPI";

   if (!File::exists (botPath)) {
      MF_Log ("ERROR: Unable to locate YaPB DLL at: %s\n", botPath.chars ());
      disableNatives ();

      return;
   }

   if (!botdll_.load (botPath)) {
      MF_Log ("ERROR: Unable to open YaPB DLL at: %s\n", botPath.chars ());
      disableNatives ();

      return;
   }
   auto api = botdll_.resolve <Export> (exportFunction.chars ());

   if (!api) {
      MF_Log ("ERROR: Missing YaPB DLL export %s:%s()\n", botPath.chars (), exportFunction.chars ());
      disableNatives ();

      return;
   }
   api_ = api (kYaPBModuleVersion);

   if (!api) {
      MF_Log ("ERROR: No API returned from YaPB DLL %s. Probably bot api version mismatch.\n", botPath.chars ());
      disableNatives ();

      return;
   }
}

void YaPBModule::unload () {
   if (botdll_) {
      botdll_.unload ();
   }
}

void YaPBModule::disableNatives () {
   for (size_t i = 0; botNatives[i].name; ++i) {
      botNatives[i].func = [] (AMX *amx, cell *params) -> cell {
         MF_LogError (amx, AMX_ERR_NATIVE, "Native is unavailable. YaPB DLL isn't loaded.");
         return 0;
      };
   }
}

// amxx exports
void OnAmxxAttach () {
   yapb.load ();
}

void OnAmxxDetach () {
   yapb.unload ();
}

// override new/delete globally, need to be included in .cpp file
#include <crlib/cr-override.h>
