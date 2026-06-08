<img src="../../Assets/PortalSolverLogo.png" width="96m" align="left" hspace="15" alt="Portal Solver Logo">

## [Portal Solver Editor SDK](../../PortalSolverEditorSDK.md) > [Reference](../Index.md) > [Utilities](Index.md) > [Register]()

<br>

## PseUtilities::Register::

| Сигнатура                                               | Тип возвращаемого значения | Аргументы                                         | Описание                                                                             |
|---------------------------------------------------------|----------------------------|---------------------------------------------------|--------------------------------------------------------------------------------------|
| **setBit<offset>(outRegister, bInValue)**               | `void`                     | `PseRegister& outRegister`, `const bool bInValue` | Записывает один бит по указанному смещению                                           |
| **getBit<offset>(inRegister)**                          | `bool`                     | `const PseRegister inRegister`                    | Считывает один бита по указанному смещению                                           |
| **set<T, offset>set(outRegister, InValue)**             | `void`                     | `PseRegister& outRegister`, `const T InValue`     | Упаковывает тип `T` по указанному смещению                                           |
| **get<T, offset>get(inRegister)**                       | `T`                        | `const PseRegister inRegister`                    | Распаковывает тип `T` по указанному смещению                                         |
| **set<T, offset, bitCount>setAs(outRegister, InValue)** | `void`                     | `PseRegister& outRegister`, `const T InValue`     | Упаковывает указанное количество бит `bitCount` из типа `T` по указанному смещению   |
| **set<T, offset, bitCount>setAs(inRegister)**           | `T`                        | `const PseRegister inRegister`                    | Распаковывает указанное количество бит `bitCount` из типа `T` по указанному смещению |
