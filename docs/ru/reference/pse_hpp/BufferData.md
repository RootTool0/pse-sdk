<img src="../../../assets/portal_solver_logo.png" width="96m" align="left" hspace="15" alt="Portal Solver Logo">

## [Portal Solver Editor SDK](../../pse_sdk.md) > [Reference](../index.md) > [Pse.hpp](index.md) > [BufferData]()

<br>

## PseUtilities::BufferData::

| Сигнатура                                  | Тип возвращаемого значения | Аргументы                                          | Описание                                                                                                                      |
|--------------------------------------------|----------------------------|----------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------|
| **set<T, offset>(outBufferData, inValue)** | `void`                     | `PseBufferData& outBufferData`, `const T& inValue` | Записывает структуру _(или значение)_ типа `T` внутрь `PseBufferData` по указанному смещению _(смещение считается от `data`)_ |
| **get<T, offset>(inBufferData)**           | `T`                        | `const PseBufferData& inBufferData`                | Считывает структуру _(или значение)_ типа `T` внутри `PseBufferData` по указанному смещению _(смещение считается от `data`)_  |
