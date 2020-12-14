//
// YaPB - Counter-Strike Bot based on PODBot by Markus Klinge.
// Copyright © 2004-2020 YaPB Project <yapb@jeefo.net>.
//
// SPDX-License-Identifier: MIT
//

#if defined _yapb_included
	#endinput
#endif

#define _yapb_included

#pragma reqlib yapb
#if !defined AMXMODX_NOAUTOLOAD
   #pragma loadlib yapb
#endif

/*
 * Invalid node index
 *
 */
#define INVALID_NODE_INDEX -1

/**
 * Defines for nodes flags field (32 bits are available)
 */
enum NodeFlag {
   Lift = (1 << 1), // wait for lift to be down before approaching this node
   Crouch = (1 << 2), // must crouch to reach this node
   Crossing = (1 << 3), // a target node
   Goal = (1 << 4), // mission goal point (bomb, hostage etc.)
   Ladder = (1 << 5), // node is on ladder
   Rescue = (1 << 6), // node is a hostage rescue point
   Camp = (1 << 7), // node is a camping point
   NoHostage = (1 << 8), // only use this node if no hostage
   DoubleJump = (1 << 9), // bot help's another bot (requster) to get somewhere (using djump)
   Narrow = (1 << 10), // node is inside some small space (corridor or such)
   Sniper = (1 << 28), // it's a specific sniper point
   TerroristOnly = (1 << 29), // it's a specific terrorist point
   CTOnly = (1 << 30),  // it's a specific ct point
}

/*
* Gets the YaPB version string.
*
* @param output     Buffer to copy version to.
* @param len        Maximum buffer size.
*
* @noreturn
*/
native yb_get_bot_version (output[], len);

/**
 * Retrieves graph node index near the location.
 *
 * @param origin    Vector of the location.
 *
 * @return          Graph node index.
 */
native yb_get_nearest_node (const origin[3]);

/**
 * Retrieves graph node location via it's index.
 *
 * @param node      Graph node index.
 * @param origin    Array to store graph node origin.
 *
 * @noreturn
 */
native yb_get_node_origin (node, origin[3]);

/**
 * Retrieves graph node flags via it's index.
 *
 * @param node      Graph node index.
 *
 * @return          Graph node flags.
 */
native NodeFlag:yb_get_node_flags (node);

/**
 * Retrieves node index bot currently standing.
 *
 * @param index     Bot index.
 *
 * @note            Can return INVALID_NODE_INDEX if bot currently dead / camping.
 *
 * @return          Graph node index.
 */
native yb_get_bot_node (index);

/**
 * Retrieves node index bot currently going to (goal).
 *
 * @param index     Bot index.
 *
 * @note            Can return INVALID_NODE_INDEX if bot currently dead / camping.
 *
 * @return          Graph node index.
 */
native yb_get_bot_goal (index);

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
native yb_set_bot_goal_node (index, node);

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
native yb_set_bot_goal_origin (index, const origin[3]);

/**
 * Checks whether bots are added on server.
 *
 * @return          True if bots are added to server, false otherwise.
 */
native bool:yb_game_has_bots ();

/**
 * Checks whether current map has graph file.
 *
 * @return          True graph is available, false otherwise.
 */
native bool:yb_has_graph ();

/**
 * Checks if player is an YaPB bot.
 *
 * @param index     Player index.
 *
 * @return          True if players is YaPB bot, false otherwise.
 */
native bool:yb_is_user_bot (index);

/**
 * Checks if graph node index is valid.
 *
 * @param node      Graph node index.
 *
 * @return          True if graph node is valid, false otherwise.
 */
native bool:yb_is_node_valid (node);


 