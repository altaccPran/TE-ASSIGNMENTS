# Assignment 4 — Dynamic retro To-Do list

A small static **HTML / CSS / JavaScript** page: a to-do list with **browser storage**, **prompt / alert / confirm** dialogs, and **email validation**. The look is deliberately **plain** (early-2000s style: gray background, simple borders, Arial/Tahoma-style buttons), not a modern “app” UI.

## Files

| File | Purpose |
|------|---------|
| `index.html` | Page structure: heading, welcome line, task count / status, email field, list area, buttons. Includes HTML comments marking where dynamic content appears. |
| `style.css` | External stylesheet: page background, bordered content box, link colors, button and list styling. |
| `script.js` | All behavior: load/save tasks, render list, email check, prompts and alerts. |

Open **`index.html`** in a desktop browser (Chrome, Firefox, Edge, Safari). No server or build step is required.

## What was implemented

- **UI (HTML + CSS):** Single page with a linked external stylesheet; labels and inputs; unordered list filled by script.
- **Dynamic behavior (JavaScript):**
  - Tasks stored in **`localStorage`** as JSON (`retro_todo_tasks_v1`). Each task has **text**, **priority**, and **done** (strike-through when done).
  - **Add Task** uses **`prompt()`** for task text and priority; **`alert()`** if input is invalid.
  - **Click** a task to toggle done/undone; **double-click** to delete (**`confirm()`** first).
  - **Clear All** uses **`confirm()`** and clears stored tasks.
  - **Save Email** runs simple validation; **`alert()`** on failure or success.
  - On first visit, an optional **`prompt()`** asks for a **name**; the main heading can change to `YourName's To-Do List` (stored so it does not ask every time).
  - **Welcome line** shows today’s date (set when the page loads).
  - **Tasks:** count and **Last saved** time update when the list is saved (timestamp stored in `localStorage`).

## How to present this to your professor

Use this as a short **live demo checklist** (about 2–3 minutes).

1. **Open the page**  
   Double-click `index.html` or use **File → Open** in the browser. Point out the **gray, simple layout** and that styling comes from **`style.css`**.

2. **Optional name prompt**  
   If the browser has never stored a name for this page, a **prompt** appears. Enter a name (or cancel / leave blank) and show how the **title** updates or stays default.

3. **Welcome line**  
   Show the line with **today’s date** (filled by JavaScript, not written in the HTML file).

4. **Add tasks**  
   Click **Add Task**, enter a task and a numeric priority in the **prompts**. Show new rows in the list and the **Tasks** count increasing.

5. **Persistence**  
   **Reload the page** (F5 or Cmd+R). Tasks should **still be there**. Say that data is saved in **`localStorage`** in this browser.

6. **Complete a task**  
   **Single-click** a task to strike it through; reload again to show **done** state is saved.

7. **Delete one task**  
   **Double-click** a task, accept **confirm**, show the list and count update.

8. **Clear all**  
   **Clear All**, confirm, show empty list; reload to show it **stays** empty.

9. **Email validation**  
   Type an invalid email, click **Save Email** → **`alert`**. Type a plausible email → success **`alert`**.

10. **Code walkthrough (optional)**  
    Open `script.js` and briefly show: **`localStorage`**, **`JSON.stringify` / `parse`**, **`prompt` / `alert` / `confirm`**, and functions like `addTask`, `saveTasks`, `renderTasks`.

### If something looks “wrong” during demo

- **Name prompt every time:** Clear site data for this file or remove `localStorage` keys starting with `retro_todo_` in DevTools → Application → Local Storage (wording varies by browser).
- **Tasks don’t persist:** Private/incognito mode, strict settings, or storage disabled can block `localStorage`; use a normal window.

## Resetting for a clean demo

To reset name + tasks + saved time in the browser console (on the page):

```javascript
localStorage.removeItem('retro_todo_tasks_v1');
localStorage.removeItem('retro_todo_display_name');
localStorage.removeItem('retro_todo_saved_at');
location.reload();
```

Then reload the page for a fresh prompt and empty list.
