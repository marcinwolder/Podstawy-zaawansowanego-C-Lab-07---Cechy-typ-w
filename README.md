# Lab 07 - Cechy typów: zadania

| # | Poziom | Temat | Sekcje |
|---|---|---|---|
| 1 | najprostsze | własne predykaty: `IsPointer`, `IsSame`, `IsVoid`, `IsArray` + skróty `_v` | 2.2, 2.3, 5 |
| 2 | najprostsze | własne przekształcenia: `RemoveConst`, `RemoveReference`, `AddPointer`, `RemoveConstRef` + skróty `_t` | 2.3, 3.3, 6.3 |
| 3 | konkretne | `describe_type<T>()` - łańcuch `if constexpr`, `always_false`, przekazanie przez wartość | 3.1, 3.3, 4.2, 6.1, 6.4 |
| 4 | praktyczne | przenieść czy skopiować? Realokacja jak w `std::vector` (`noexcept`, `std::conditional_t`) | 4.1, 4.3 |
| 5 | praktyczne | przeciążenia z `enable_if` (SFINAE) i czytelne błędy ze `static_assert` | 4.1, 4.4 |

## Szybki start

Wymagania: CMake >= 3.20, kompilator C++20 (GCC 10+, Clang 10+, MSVC 2019 16.11+), git i internet przy pierwszej konfiguracji. CMake pobiera GoogleTest, jeśli nie ma go w systemie.

1. Otwórz plik `zadania/zadanieN.hpp`. Zmieniasz **tylko** pliki w `zadania/`.
2. Wykonaj kroki z komentarzy `TODO`.
3. Zbuduj projekt i uruchom testy:

```bash
cmake -S . -B build                                   # raz, na początku
cmake --build build                                   # po każdej zmianie
./build/task3                                         # testy zadania 3 (analogicznie 1-5)
./build/task3 --gtest_filter='Task3_DescribeType.*'       # tylko wybrane testy
ctest --test-dir build -L task3 --output-on-failure   # zadanie 3 razem z testami "ma się nie kompilować"
ctest --test-dir build -L task --output-on-failure    # wszystkie zadania
```

Windows (Visual Studio): `cmake --build build --config Release`, pliki są w `build\Release\`, testy: `ctest --test-dir build -C Release -L task`.

> [!WARNING]
> Najpierw `cmake --build`, potem `ctest`. 
> `ctest` nie kompiluje testów. Po zmianie w `zadanieN.hpp` zawsze uruchom `cmake --build build`. Wyjątek: testy "ma się nie kompilować" kompiluje sam `ctest`.

## Jak czytać wynik

Szkielety się kompilują. Na początku testy działają, ale większość **nie przechodzi**. Zadanie jest zrobione, gdy `ctest -L taskN` pokazuje `100% tests passed`.

```
[ RUN      ] Task2_RemoveConst.Basics
testy/test_zadanie2.cpp:11: Failure
Value of: (test_helpers::same_types<RemoveConst<const int>::type, int>())
  Actual: false (
  actual:   const int
  expected: int)
Expected: true
[  FAILED  ] Task2_RemoveConst.Basics (0 ms)
```

Testy są dwóch rodzajów:

1. **Zwykłe testy** (`testy/test_zadanieN.cpp`, GoogleTest). Cechy typów liczy kompilator, ale wynik sprawdzamy w czasie wykonania (`EXPECT_TRUE`, `EXPECT_EQ`). Dlatego widzisz **wszystkie** błędy naraz.
2. **Testy "ma się nie kompilować"** (`bledy/zadanieN_*.cpp`). Część zadań wymaga błędu kompilacji z Twoim komunikatem ze `static_assert`. `ctest` kompiluje taki plik i sprawdza dwie rzeczy: kompilacja kończy się błędem, a w błędzie jest tekst z linii `// EXPECTED_ERROR:`. W `ctest` te testy mają nazwy `taskN.MustNotCompile.*`.

```
task5.MustNotCompile.swap_values ...***Failed
  FAIL bledy/zadanie5_swap_values.cpp
    the code compiled, but it must fail with the error: "swap_values: type must be copyable"
```

> [!TIP]
> Plik testów się nie kompiluje? 
> `cmake --build build` pokaże błąd kompilatora. Zwykle to błąd w Twoim `zadanieN.hpp`. Zbuduj tylko jedno zadanie, aby inne nie zasłaniały błędu: `cmake --build build --target task3`.

## Zadania

### Zadanie 1 (najprostsze): własne predykaty

Plik: `zadania/zadanie1.hpp`. Materiały: sekcja 2.2, 2.3 i 5.

1. Napisz `IsPointer<T>`: szablon ogólny dziedziczy z `std::false_type`, specjalizacja dla `T*` z `std::true_type`.
2. Napisz `IsSame<T, U>` przez specjalizację `IsSame<T, T>`.
3. Napisz `IsVoid<T>` przez pełną specjalizację `template <>`.
4. Napisz `IsArray<T>`. Tablica ma dwa kształty: `T[N]` i `T[]`.
5. Uzupełnij skróty `IsPointer_v`, `IsSame_v`, `IsVoid_v`, `IsArray_v`.

Nie używaj `std::is_pointer` i podobnych. Testy porównują Twoje cechy z nimi.

### Zadanie 2 (najprostsze): własne przekształcenia typów

Plik: `zadania/zadanie2.hpp`. Materiały: sekcja 2.3, 3.3 i 6.3.

1. Napisz `RemoveConst<T>` (specjalizacja dla `const T`).
2. Napisz `RemoveReference<T>` (specjalizacje dla `T&` i `T&&`).
3. Uzupełnij aliasy `RemoveConst_t` i `RemoveReference_t`.
4. Napisz `AddPointer<T>`. Dla referencji wynik to wskaźnik do typu bez referencji: `AddPointer_t<int&>` to `int*`.
5. Napisz `RemoveConstRef<T>`: `const int&` daje `int`. Test `Task2_Pitfall63.RemoveConstDoesNotWorkThroughReference` pokazuje, dlaczego kolejność cech ma znaczenie.

### Zadanie 3 (konkretne): `describe_type`

Plik: `zadania/zadanie3.hpp`. Materiały: sekcja 3.1, 3.3, 4.2, 6.1 i 6.4.

1. Napisz `describe_type<T>()`. Funkcja zwraca opis typu słowami, np. `describe_type<const char*>()` to `"pointer to const char"`. Pełna tabela jest w szkielecie.
2. Typ spoza tabeli ma dać błąd kompilacji ze `static_assert(always_false<T>, "describe_type: unsupported type")`. Sprawdza to `bledy/zadanie3_unsupported.cpp`.
3. Napisz `describe_by_value(T)` i `describe_by_reference(T&)`. Porównaj wyniki w teście `Task3_Passing.ByValueAndByReference`.

Pytanie do przemyślenia: dlaczego `describe_by_value(array)` zwraca `"pointer to integer"`, a `describe_by_reference(array)` zwraca `"array 3 x integer"`?

### Zadanie 4 (praktyczne): przenieść czy skopiować?

Plik: `zadania/zadanie4.hpp`. Materiały: sekcja 4.1 i 4.3.

Gdy `std::vector` nie ma miejsca, tworzy większy bufor i przenosi do niego elementy. Jeśli przenoszenie może rzucić wyjątek, wektor kopiuje elementy. Napiszesz ten sam mechanizm.

1. Napisz alias `MoveResult<T>` przez `std::conditional_t`: `T&&` (przenieś) albo `const T&` (skopiuj).
2. Napisz `reallocate(old_vec)`: nowy wektor, `reserve`, `push_back(move_or_copy(element))`.
3. Dodaj `static_assert` z komunikatem `"reallocate: type must be movable or copyable"`. Sprawdza to `bledy/zadanie4_mutex.cpp`.

Testy liczą kopie i przeniesienia trzech typów: `Safe` (move `noexcept`), `Risky` (move bez `noexcept`) i `MoveOnly`. Test `Task4_StdVector.DoesTheSame` przechodzi od razu. Pokazuje, że `std::vector` zachowuje się tak samo.

### Zadanie 5 (praktyczne): `enable_if` i czytelne błędy

Plik: `zadania/zadanie5.hpp`. Materiały: sekcja 4.1 i 4.4.

1. Napisz `average(v)` z `enable_if` w typie zwracanym. Funkcja ma istnieć tylko dla wektorów liczb.
2. Zamień szablon `to_text` na cztery przeciążenia z `enable_if` jako parametrem szablonu: `bool`, inne całkowite, zmiennoprzecinkowe, tekst. Dla innych typów funkcji ma "nie być".
3. Dodaj do `swap_values(a, b)` `static_assert` z komunikatem `"swap_values: type must be copyable"`. Sprawdza to `bledy/zadanie5_swap_values.cpp`.

Testy sprawdzają też, że dla złych typów wywołanie **się nie kompiluje**. Do tego służy detekcja przez `std::void_t` na początku `testy/test_zadanie5.cpp`. Przeczytaj ją: to ta sama technika SFINAE, której używasz w zadaniu.

> [!WARNING]
> `bool` jest typem całkowitym. 
> Jeśli nie wykluczysz go z przeciążenia "inne całkowite", wywołanie `to_text(true)` pasuje do dwóch przeciążeń. Test pokaże wtedy: `to_text(bool) does not exist or is ambiguous`.

## Wymagania

Minimalne wersje dla C++17: GCC 8, Clang 7, MSVC 2017 (15.8). Dla C++20: GCC 10, Clang 10, MSVC 2019 (16.11).

## Struktura katalogu

```
zadania/         szkielety z TODO - tu piszesz rozwiązanie
testy/           testy GoogleTest (NIE MODYFIKUJ) i pomocniczy nagłówek type_eq.hpp
bledy/           pliki, które mają się NIE skompilować
cmake/           skrypt testów "ma się nie kompilować" (uruchamia go ctest)
CMakeLists.txt   cele task1..5 i testy dla ctest
build/           pliki wykonywalne (tworzy je CMake)
```
