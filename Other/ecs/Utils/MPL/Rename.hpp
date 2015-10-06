#pragma once

namespace ecs
{
    namespace MPL
    {
        namespace Impl
        {
            template <template <typename...> class TNewName, typename T>
            struct RenameHelper;

            // "Renames" `TOldName<Ts...>` to `TNewName<Ts...>`.
            template <template <typename...> class TNewName,
                template <typename...> class TOldName, typename... Ts>
            struct RenameHelper<TNewName, TOldName<Ts...>>
            {
                using type = TNewName<Ts...>;
            };

            template <template <typename...> class TNewName, typename T>
            using Rename = typename RenameHelper<TNewName, T>::type;
        }
    }
}