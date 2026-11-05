#pragma once

#include <cstddef>
#include <type_traits>

// Zadanie 1 (najprostsze): własne cechy typów - predykaty (materiały: sekcja 2.2 i 5).
// Schemat jest zawsze ten sam:
//   1. szablon ogólny dziedziczy z std::false_type (odpowiedź domyślna),
//   2. specjalizacja częściowa dla wybranego kształtu typu dziedziczy z std::true_type.
// Nie używaj gotowych cech z <type_traits> (std::is_pointer itd.) - napisz je sam.

// TODO 1: IsPointer<T> - czy T jest wskaźnikiem?
//         Dodaj specjalizację dla T* (sekcja 5).
template <typename T> struct IsPointer : std::false_type
{
};

// TODO 2: IsSame<T, U> - czy T i U to ten sam typ?
//         Wskazówka: specjalizacja z JEDNYM parametrem, użytym dwa razy: IsSame<T, T>.
template <typename T, typename U> struct IsSame : std::false_type
{
};

// TODO 3: IsVoid<T> - czy T to void?
//         Wskazówka: pełna specjalizacja template <> struct IsVoid<void> ...
template <typename T> struct IsVoid : std::false_type
{
};

// TODO 4: IsArray<T> - czy T jest tablicą?
//         Tablica ma dwa kształty: o znanym rozmiarze int[5] i o nieznanym int[].
//         Potrzebujesz DWÓCH specjalizacji. Rozmiar to parametr szablonu: std::size_t N.
template <typename T> struct IsArray : std::false_type
{
};

// TODO 5: skróty _v (sekcja 2.3). Zamień `false` na ::value odpowiedniej cechy.
template <typename T> inline constexpr bool IsPointer_v = false;

template <typename T, typename U> inline constexpr bool IsSame_v = false;

template <typename T> inline constexpr bool IsVoid_v = false;

template <typename T> inline constexpr bool IsArray_v = false;
