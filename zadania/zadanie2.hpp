#pragma once

#include <type_traits>

// Zadanie 2 (najprostsze): własne cechy przekształcające typ (materiały: sekcja 2.3, 3.3 i 6.3).
// Cecha przekształcająca zwraca nowy typ jako składową `using type = ...;`.
// Skrót _t to alias: template <typename T> using X_t = typename X<T>::type;
// Nie używaj gotowych std::remove_const, std::remove_reference itd. - napisz je samodzielnie.
// Wolno Ci używać cech z TEGO pliku w kolejnych cechach.

// TODO 1: RemoveConst<T> - usuń const z najwyższego poziomu.
//         RemoveConst<const int>::type to int, RemoveConst<int>::type to int.
//         Wskazówka: specjalizacja dla const T.
template <typename T> struct RemoveConst
{
    using type = T;
};

// TODO 2: RemoveReference<T> - usuń referencję & albo &&.
//         Potrzebujesz dwóch specjalizacji: T& i T&&.
template <typename T> struct RemoveReference
{
    using type = T;
};

// TODO 3: skróty _t (sekcja 2.3). Zamień `T` na typename ...::type odpowiedniej cechy.
template <typename T> using RemoveConst_t = T;

template <typename T> using RemoveReference_t = T;

// TODO 4: AddPointer<T> - dodaj wskaźnik.
//         Uwaga: wskaźnik do referencji nie istnieje (int&* to błąd).
//         Dla referencji wynikiem ma być wskaźnik do typu bez referencji:
//         AddPointer<int&>::type to int*, tak samo jak w std::add_pointer.
template <typename T> struct AddPointer
{
    using type = T;
};

template <typename T> using AddPointer_t = typename AddPointer<T>::type;

// TODO 5: RemoveConstRef<T> - usuń referencję ORAZ const, np. const int& -> int.
//         Pułapka z sekcji 6.3: const w `const int&` nie jest "na wierzchu",
//         więc kolejność wywołań cech ma znaczenie. Sprawdź oba warianty.
template <typename T> struct RemoveConstRef
{
    using type = T;
};

template <typename T> using RemoveConstRef_t = typename RemoveConstRef<T>::type;
