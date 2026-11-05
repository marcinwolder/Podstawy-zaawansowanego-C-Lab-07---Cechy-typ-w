#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

// Zadanie 4 (praktyczne): przenieść czy skopiować? (materiały: sekcja 4.1 i 4.3).
// Napiszesz to, co robi std::vector, gdy brakuje mu miejsca: tworzy większy bufor
// i przenosi do niego elementy. Przenoszenie jest szybsze, ale jeśli może rzucić
// wyjątek, to w połowie pracy stare dane byłyby już zepsute. Dlatego:
//   - przenoszenie noexcept                         -> przenieś,
//   - przenoszenie bez noexcept, ale jest kopiowanie -> skopiuj (bezpiecznie),
//   - przenoszenie bez noexcept i brak kopiowania    -> przenieś (nie ma wyboru).

// TODO 1: MoveResult<T> - typ zwracany przez move_or_copy:
//           T&&       -> gdy trzeba przenieść,
//           const T&  -> gdy trzeba skopiować (z const T& da się tylko skopiować).
//         Wskazówki:
//           - std::conditional_t<warunek, A, B> to "if" dla typów: daje A, gdy warunek
//             jest prawdziwy, w przeciwnym razie B,
//           - std::is_nothrow_move_constructible_v<T>, std::is_copy_constructible_v<T>.
template <typename T> using MoveResult = T &&;

// Gotowe: std::move(x) daje T&&. Gdy MoveResult<T> to const T&, ta sama
// referencja wiąże się jako const T&, więc przy push_back zadziała konstruktor kopiujący.
// Tak działa std::move_if_noexcept z <utility>.
template <typename T> MoveResult<T> move_or_copy(T &x) noexcept
{
    return std::move(x);
}

// TODO 2: reallocate(old_vec) - nowy wektor z elementami starego, tak jak przy realokacji:
//           1. utwórz pusty std::vector<T>,
//           2. zarezerwuj miejsce: 2 * old_vec.size(), ale co najmniej 1 (reserve),
//           3. każdy element starego dodaj przez push_back(move_or_copy(element)),
//           4. zwróć nowy wektor.
// TODO 3: na początku reallocate dodaj static_assert (sekcja 4.1):
//           warunek:   std::is_move_constructible_v<T>
//                      (to true także dla typów tylko kopiowalnych: const T& przyjmie T&&),
//           komunikat: "reallocate: type must be movable or copyable".
template <typename T> std::vector<T> reallocate([[maybe_unused]] std::vector<T> &old_vec)
{
    return {};
}
