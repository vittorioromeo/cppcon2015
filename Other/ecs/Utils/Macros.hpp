#pragma once

// Shortcut for `std::forward`.
#define ECS_FWD(mX) \
	::std::forward<decltype(mX)>(mX)

// Syntactic sugar to retrieve the type from a MPL Type wrapper.
#define ECS_TYPE(mX) \
	typename decltype(mX)::type

// Pastes two preprocessor tokens together.
#define ECS_CAT(mA0, mA1) \
	mA0 ## mA1

/// Micro-optimization telling the compiler that this condition is more likely to happen than the `else` branch.
#define ECS_LIKELY(mCondition) \
	__builtin_expect(!!(mCondition), 1)

/// Micro-optimization telling the compiler that this condition is less likely to happen than the `else` branch.
#define ECS_UNLIKELY(mCondition) \
	__builtin_expect(!!(mCondition), 0)