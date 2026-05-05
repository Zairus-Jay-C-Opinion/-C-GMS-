# Dynamic Grade System (DGS)
A console-based grade monitoring program written in C++ that allows students to track their academic performance across multiple subjects, compute weighted averages, and project the scores needed to reach a target grade.

---

## Features

- **User Authentication** — Register and login with a username and password. Login is limited to 3 attempts before the program exits.
- **Subject Management** — Create multiple subjects, each identified by a unique ID.
- **Component Management** — Add graded components (e.g. Quiz, Exam, Activity) to each subject with a defined weight percentage and optional total items.
- **Weight Validation** — Components must sum to exactly 100% before scores can be entered.
- **Score Entry** — Enter raw scores (if total items is defined) or percentage scores per component. The program shows how much of the component weight you earned upon saving.
- **Required Scores Projection** — Given at least one scored component, the program projects the score you must hit equally across all remaining assessments to reach a target numeric grade. Also shows the best grade still achievable.
- **Subject Average** — Displays a full score breakdown per component with weighted contributions and a final numeric grade if all components are scored.
- **Final Average** — Computes the simple average of numeric grades across all fully scored subjects.
- **Academic Performance** — Full overview of all subjects showing status, weighted averages, numeric grades, and an overall final standing.

---

## Grade Conversion Table

| Numeric Grade | Minimum % | Maximum % |
|---------------|-----------|-----------|
| 1.00          | 98        | 100       |
| 1.25          | 94        | 97        |
| 1.50          | 90        | 93        |
| 1.75          | 88        | 89        |
| 2.00          | 85        | 87        |
| 2.25          | 83        | 84        |
| 2.50          | 80        | 82        |
| 2.75          | 78        | 79        |
| 3.00          | 75        | 77        |
| 5.00          | 0         | 74        |

---

## Performance Classification

| Classification | Numeric Grade Range |
|----------------|---------------------|
| Outstanding    | 1.00 – 1.50         |
| Safe           | 1.75 – 2.50         |
| At Risk        | 2.75 – 3.00         |
| Critical       | 5.00                |

---

## How Grades Are Computed

**Component percentage:**
- If total items is defined: `(score / total_items) × 100`
- If not defined: the entered value is treated directly as a percentage

**Subject weighted average:**
`sum of (component_percentage × component_weight / 100)` across all components

**Final average:**
Simple average of all fully scored subjects' numeric grades

---

## Workflow

```
Register → Login → Dashboard
```

Inside the dashboard, the intended order of operations per subject is:

```
1. Create Subject
2. Add Component       ← repeat until weights sum to 100%
3. Validate Weights
4. Add Scores          ← unlocked after validation
5. Required Scores     ← available once at least one score is entered
6. Subject Average     ← shows full breakdown
7. Final Average       ← shows overall average across subjects
8. Academic Performance
```

## Notes

- Subject names support spaces (e.g. `CC 103`).
- A component's total items can be defined either when adding the component or when entering its score for the first time.
- Scores of exactly `0` are valid and treated as scored.
- The program does not persist data between sessions — all data is lost when the program exits.

---

## Developer

**Zairus Jay C. Opinion | CS-1203**
BS Computer Science — Batangas State University TNEU
CC 103 — Data Structure and Algorithm
