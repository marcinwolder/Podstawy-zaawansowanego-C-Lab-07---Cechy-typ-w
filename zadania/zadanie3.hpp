#pragma once

#include <string>
#include <type_traits>

// Zadanie 3 (konkretne): opis typu słowami (materiały: sekcja 3.1, 3.3, 4.2, 6.1 i 6.4).
// Tu używasz już gotowych cech z <type_traits> (std::is_integral_v, std::remove_const_t ...).

// Gotowe (sekcja 6.4): wartość zależy od T, więc static_assert(always_false<T>, ...)
// zgłasza błąd tylko wtedy, gdy kompilator naprawdę wejdzie do tej gałęzi.
template <typename T> inline constexpr bool always_false = false;

// TODO 1: describe_type<T>() - zwróć opis typu T według tabeli (sprawdzaj w tej kolejności):
//
//   referencja        -> "reference to " + opis typu bez referencji
//   const             -> "const " + opis typu bez const
//   void              -> "void"
//   bool              -> "bool"
//   char              -> "char"
//   inne całkowite    -> "integer"
//   zmiennoprzecinkowe -> "floating point"
//   std::string       -> "string"
//   wskaźnik          -> "pointer to " + opis typu wskazywanego
//   tablica T[N]      -> "array N x " + opis typu elementu
//   wszystko inne     -> błąd kompilacji: static_assert z komunikatem "describe_type: unsupported type"
//
// Przykłady: describe_type<const char*>()  == "pointer to const char"
//            describe_type<int* const>()   == "const pointer to integer"
//            describe_type<double[4]>()    == "array 4 x floating point"
//
// Wskazówki:
//   - użyj łańcucha if constexpr / else if constexpr (sekcja 4.2), a w opisach złożonych
//     wywołaj describe_type rekurencyjnie dla "mniejszego" typu,
//   - bool i char to też typy całkowite - dlatego są w tabeli wyżej,
//   - std::is_const_v<const int&> to false (sekcja 6.3). Dlatego referencja jest pierwsza,
//   - przydadzą się: std::remove_reference_t, std::remove_const_t, std::remove_pointer_t,
//     std::extent_v<T> (rozmiar N tablicy) i std::remove_extent_t<T> (typ elementu tablicy).
template <typename T> std::string describe_type()
{
    return "?";
}

// TODO 2: (sekcja 6.1) opisz typ ARGUMENTU funkcji, czyli to, co kompilator wydedukował jako T.
//         Obie funkcje mają jedną linię: return describe_type<T>();
//         Potem porównaj wyniki w testach: dlaczego dla tej samej zmiennej są różne?
template <typename T> std::string describe_by_value(T)
{ // przekazanie przez wartość
    return "?";
}

template <typename T> std::string describe_by_reference(T &)
{ // przekazanie przez referencję
    return "?";
}
