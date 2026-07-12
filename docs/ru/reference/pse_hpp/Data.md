<img src="../../../assets/portal_solver_logo.png" width="60m" align="left" alt="Portal Solver Logo">

## [Portal Solver Editor SDK](../../pse_sdk.md) > [Reference](../index.md) > [Pse.hpp](index.md) > [Data]()

<br>

## pse::Data::

Пространство имен `pse::Data::` содержит набор **шаблонных утилит** _(хелперов)_, предназначенных для быстрой и безопасной работы со структурами `PseData`. Они позволяют безопасно читать и записывать данные произвольных типов в `PseData` _(в поле `uint8_t data[60]`)_

| Сигнатура                                                                      | Тип возвращаемого значения | Описание                                                                                                    |
|--------------------------------------------------------------------------------|----------------------------|-------------------------------------------------------------------------------------------------------------|
| `template<uint8_t offset, typename T> set(PseData& outData, const T& inValue)` | `void`                     | Изменяет объект `outData`, копируя в него значение `inValue` типа `T` по указанному смещению `offset`       |
| `template<uint8_t offset, typename T> get(const PseData& inData)`              | `T`                        | Считывает объект `inData`, извлекает и возвращает из него значение типа `T` по указанному смещению `offset` |
