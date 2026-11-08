# Validate Parentheses — Worked Example

Problem: https://neetcode.io/problems/validate-parentheses

**Given:** a string `s` containing only the characters `(`, `)`, `{`, `}`, `[`, `]`.
**Determine:** whether `s` is valid, where valid means:
1. Every open bracket is closed by the *same type* of bracket.
2. Open brackets are closed in the *correct order*.
3. Every close bracket has a matching open bracket earlier in the string.

---

## Step 1 — Pick the data structure

The key observation: the **most recently opened** bracket must be the
**next one closed**. "Most recent first" is exactly what a **stack**
gives you (LIFO), so:

- Push every **open** bracket onto a stack.
- When a **close** bracket arrives, it must match whatever is on **top**
  of the stack. If it matches, pop it. If it doesn't match (or the
  stack is empty), the string is invalid immediately.
- At the end, the string is valid only if the stack is **empty**
  (every open bracket got closed).

## Step 2 — Set up the lookup table

Define a map from closing bracket → its matching opening bracket:

| close | open |
|-------|------|
| `)`   | `(`  |
| `]`   | `[`  |
| `}`   | `{`  |

```
pairs = { ')': '(', ']': '[', '}': '{' }
stack = []
```

## Step 3 — Walk the string, one character at a time

### Worked (valid) example: `s = "{[()]}"`

| i | char | is open? | action                                   | stack (top →left) |
|---|------|----------|-------------------------------------------|--------------------|
| 0 | `{`  | yes      | push `{`                                   | `{`                |
| 1 | `[`  | yes      | push `[`                                   | `[ {`              |
| 2 | `(`  | yes      | push `(`                                   | `( [ {`             |
| 3 | `)`  | no       | top is `(`, `pairs[')'] == '('` → match, pop | `[ {`              |
| 4 | `]`  | no       | top is `[`, `pairs[']'] == '['` → match, pop | `{`                |
| 5 | `}`  | no       | top is `{`, `pairs['}'] == '{'` → match, pop | *(empty)*          |

End of string. **Stack is empty → `s` is valid.** ✅

### Worked (invalid) example: `s = "([)]"`

| i | char | is open? | action                                               | stack (top → left) |
|---|------|----------|--------------------------------------------------------|---------------------|
| 0 | `(`  | yes      | push `(`                                                | `(`                 |
| 1 | `[`  | yes      | push `[`                                                | `[ (`               |
| 2 | `)`  | no       | top is `[`, but `pairs[')'] == '('` ≠ `[` → **mismatch** | `[ (`  (stop here)  |

We can return `false` immediately — no need to read the rest of the
string. **`s` is invalid.** ❌ (The `(` opened at index 0 is still
waiting to be closed when `)` tries to close the `[` instead.)

### Worked (invalid, leftover) example: `s = "(]"`

Actually simpler — unmatched leftovers at the end:

`s = "(("`

| i | char | action        | stack   |
|---|------|---------------|---------|
| 0 | `(`  | push          | `(`     |
| 1 | `(`  | push          | `( (`   |

End of string, stack = `( (` → **not empty → invalid.** ❌ (Two open
brackets were never closed.)

## Step 4 — The algorithm, generalized

```
function isValid(s):
    pairs = { ')': '(', ']': '[', '}': '{' }
    stack = []

    for char in s:
        if char in pairs.values():          # it's an opening bracket
            stack.push(char)
        else:                                # it's a closing bracket
            if stack is empty or stack.top() != pairs[char]:
                return false
            stack.pop()

    return stack.is_empty()
```

## Step 5 — Complexity

- **Time:** O(n) — each character is pushed and popped at most once.
- **Space:** O(n) — worst case, the stack holds every character (e.g.
  `"((((((("`).

## Step 6 — Edge cases to sanity-check

- Empty string `""` → valid (stack starts and ends empty).
- Odd length string → always invalid (brackets can't fully pair up),
  but you don't need a special check — the algorithm handles it
  naturally via the leftover-stack / mismatch checks.
- A close bracket with nothing on the stack, e.g. `")"` → invalid,
  caught by the "stack is empty" check before popping.
