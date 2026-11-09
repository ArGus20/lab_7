# Лабораторна робота №7: Обробка виключень та тестування (Варіант 6 — ОДР)

**Виконав:** Гусар Артем Андрійович (ФІТ 3-17)  
**Тематика:** Чисельне розв'язання ОДР методом Рунге-Кутти 4-го порядку (RK4).

---

## 1. Інваріанти класу `ODETask`
- **Крок обчислень:** $h > 0$.
- **Інтервал:** $x_{end} > x_0$.
- **Передумови:** Початкові умови $(x_0, y_0)$ та значення похідних мають бути скінченними дійсними числами (`std::isfinite`).

---

## 2. Ієрархія винятків
- `std::runtime_error`
  - `ODETaskException` — базовий виняток ОДР.
    - `ODEInvalidArgumentException` — некоректні параметри ($h \le 0$, $x_{end} \le x_0$).
    - `ODEComputationException` — помилки обчислень ($NaN/Inf$, перевищення кількості кроків).

---

## 3. Гарантії безпеки (Exception Safety)
- **`setStep()` — Basic Guarantee:** інваріанти об'єкта зберігаються.
- **`computeTrajectory()` — Strong Guarantee:** реалізовано за підходом *prepare-then-commit* (обчислення у тимчасовий буфер, фіксація через `std::move`). У разі збою стан не змінюється.
- **`getTrajectorySize()`, `getTrajectory()` — No-throw Guarantee (`noexcept`).**

---

## 4. Дослідження розкручування стека (ScopeLogger)
- **Очікуваний та фактичний порядок викликів:**
  1. `[ScopeLogger] Created: runHighLevel`
  2. `[ScopeLogger] Created: processMidLevel`
  3. `[ScopeLogger] Created: stepLowLevel`
  4. `[ScopeLogger] Destroyed: stepLowLevel`
  5. `[MidLevel Catch] Error caught: ...`
  6. `[ScopeLogger] Destroyed: processMidLevel`
  7. `[ScopeLogger] Destroyed: runHighLevel`
  8. `[main Catch] ODEComputationException: ...`
- **Висновок:** Під час поширення винятку деструктори локальних об'єктів автоматично викликаються у зворотному порядку їх створення (Stack Unwinding).

---

## 5. Висновок щодо примітивних винятків (`int`, `std::string`)
Викидання примітивних типів є некоректним, оскільки вони пропускаються блоками `catch (const std::exception&)` і перехоплюються лише `catch (...)`, який не дає інформації про помилку через `what()`.

---

## 6. Збирання та запуск проєкту

```bash
mkdir build && cd build
cmake ..
cmake --build .
