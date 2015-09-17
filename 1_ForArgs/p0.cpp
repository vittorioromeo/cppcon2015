// Copyright (c) 2015 Vittorio Romeo
// License: AFL 3.0 | https://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include <iostream>

// Welcome to my talk: "`for_each_argument` explained and expanded".

// During this presentation we'll take a look at a very interesting
// code snippet, originally posted on Twitter by Sean Parent.

// {image: p0_d0}
// {image: p0_d1}

// We'll also cover a very useful C++14 standard library addition:
// "compile-time integer sequences".

// ------------------------------------------------------------------

// So, what does `for_each_argument` do?
// Well, the name is pretty self-explanatory...

// It invokes a callable object on every passed argument.

template <class F, class... Args>
void for_each_argument(F f, Args&&... args) {
    [](...){}((f(std::forward<Args>(args)), 0)...);
}

int main()
{
    // Prints "hello123".
    for_each_argument
    (
        [](const auto& x){ std::cout << x; },

        "hello",
        1,
        2,
        3
    );

    std::cout << "\n";
    return 0;
}

// That is cool. How does it work?
// Let's discover that in the next code segment.