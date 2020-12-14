//
// YaPB - Counter-Strike Bot based on PODBot by Markus Klinge.
// Copyright © 2004-2020 YaPB Project <yapb@jeefo.net>.
//
// SPDX-License-Identifier: MIT
//

#pragma once

#include <crlib/cr-basic.h>
#include <crlib/cr-complete.h>

#include <version.h>


// use all the cr-library
using namespace cr;

#include <amxx/amxxmodule.h>
#include <yapb/module.h>

class YaPBModule : public Singleton <YaPBModule> {
private:
   using Export = IYaPBModule *(*) (int);

private:
   SharedLibrary botdll_;
   IYaPBModule *api_;

public:
   YaPBModule () = default;
   ~YaPBModule () = default;

public:
   void load ();
   void unload ();
   void disableNatives ();

public:
   IYaPBModule *api () {
      return api_;
   }
};

CR_EXPOSE_GLOBAL_SINGLETON (YaPBModule, yapb);
