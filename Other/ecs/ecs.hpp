#pragma once

#include "./Utils/Utils.hpp"
#include "./Core/Core.hpp"
#include "./Handle/Handle.hpp"
#include "./Traits/Traits.hpp"
#include "./Elements/Elements.hpp"
#include "./Config/Config.hpp"
#include "./Manager/Manager.hpp"
#include "./Interface.hpp"

// TODO:
// * bitsets? or compile-time-generated hasComponent checks? (forTypes requires<...>, short circuiting)
// * vector squishing?
// * cache stuff in systems? recalculate every refresh?
// * own strong typedef?
// * serialize/deserialize state;

// TODO: Manager inner class?
// "Fat" entity handle.
// class FatHandle;

// TODO: 
// null slim/fat handles 