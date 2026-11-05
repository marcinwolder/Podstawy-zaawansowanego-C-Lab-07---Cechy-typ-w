#pragma once

#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

// Zadanie 5 (praktyczne): przeciążenia z enable_if i czytelne błędy (materiały: sekcja 4.1 i 4.4).
// Testy sprawdzają nie tylko wyniki, ale też to, że DLA ZŁYCH TYPÓW funkcji "nie ma"
// (SFINAE usuwa przeciążenie). Do tego służy detekcja przez std::void_t w pliku testów.

// TODO 1: average(v) - średnia elementów wektora jako double; pusty wektor -> 0.0.
//         Funkcja ma istnieć TYLKO dla wektorów liczb (std::is_arithmetic_v<T>).
//         Użyj enable_if w typie zwracanym (pierwsza forma z sekcji 4.4):
//           std::enable_if_t<warunek, double> average(...)
//         Wtedy average(std::vector<std::string>{}) się nie skompiluje: "no matching function".
template <typename T> double average([[maybe_unused]] const std::vector<T> &v)
{
    return 0.0;
}

// TODO 2: to_text(x) - zamiana na tekst. Zamień poniższy szablon na CZTERY przeciążenia
//         z enable_if jako parametrem szablonu (druga forma z sekcji 4.4):
//           template <typename T, std::enable_if_t<warunek, int> = 0>
//           std::string to_text(const T& x) { ... }
//
//   bool                                  -> "true" / "false"
//   inne całkowite                        -> std::to_string(x), np. 42 -> "42"
//   zmiennoprzecinkowe                    -> przez std::ostringstream, np. 1.5 -> "1.5"
//   tekst (typ zamienialny na std::string) -> w cudzysłowie, np. "abc" -> "\"abc\""
//                                            (std::is_convertible_v<T, std::string>)
//   wszystkie inne typy                   -> brak przeciążenia (ma się nie kompilować)
//
//   Uwaga: bool jest typem całkowitym. Jeśli tego nie wykluczysz, to_text(true)
//   pasuje do dwóch przeciążeń naraz i wywołanie jest niejednoznaczne.
template <typename T> std::string to_text([[maybe_unused]] const T &x)
{
    return "?";
}

// TODO 3: swap_values(a, b) działa jak my_swap z sekcji 4.1. Dodaj na początku static_assert:
//           warunek:   typ da się skopiować i przypisać
//                      (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>),
//           komunikat: "swap_values: type must be copyable".
//         Sprawdzi to bledy/zadanie5_swap_values.cpp: dla std::unique_ptr kompilator
//         ma pokazać TWÓJ komunikat, a nie długi błąd z wnętrza szablonu.
template <typename T> void swap_values(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
