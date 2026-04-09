// simple todo list — tasks kept in memory and saved to localStorage

var STORAGE_KEY = "retro_todo_tasks_v1";
var NAME_KEY = "retro_todo_display_name";
var TIME_KEY = "retro_todo_saved_at";

// each item: { text: string, priority: string, done: boolean }
var tasks = [];

function saveTasks() {
    try {
        localStorage.setItem(STORAGE_KEY, JSON.stringify(tasks));
        localStorage.setItem(TIME_KEY, new Date().toISOString());
    } catch (e) {
        alert("Could not save (storage full or disabled).");
        return;
    }
    updateTaskCount();
    updateStatusLine();
}

function loadTasksFromStorage() {
    var raw = localStorage.getItem(STORAGE_KEY);
    if (!raw) {
        tasks = [];
        return;
    }
    try {
        tasks = JSON.parse(raw);
        if (!Array.isArray(tasks)) {
            tasks = [];
        }
    } catch (e) {
        tasks = [];
    }
}

function updateTaskCount() {
    var el = document.getElementById("taskCount");
    if (el) {
        el.textContent = String(tasks.length);
    }
}

function updateStatusLine() {
    var el = document.getElementById("statusLine");
    if (!el) {
        return;
    }
    var raw = localStorage.getItem(TIME_KEY);
    if (raw) {
        el.textContent = "Last saved: " + new Date(raw).toLocaleString();
    } else {
        el.textContent = "Not saved yet";
    }
}

function renderTasks() {
    var list = document.getElementById("taskList");
    if (!list) {
        return;
    }
    list.innerHTML = "";

    for (var i = 0; i < tasks.length; i++) {
        (function (index) {
            var t = tasks[index];
            var li = document.createElement("li");
            li.textContent = t.text + " (Priority: " + t.priority + ")";
            if (t.done) {
                li.style.textDecoration = "line-through";
            }

            li.onclick = function () {
                tasks[index].done = !tasks[index].done;
                saveTasks();
                renderTasks();
            };

            li.ondblclick = function () {
                if (confirm("Delete this task?")) {
                    tasks.splice(index, 1);
                    saveTasks();
                    renderTasks();
                }
            };

            list.appendChild(li);
        })(i);
    }

    updateTaskCount();
}

// optional: ask for name once and show in title (stored in localStorage)
function maybeAskName() {
    var saved = localStorage.getItem(NAME_KEY);
    if (saved !== null) {
        if (saved.length > 0) {
            document.getElementById("pageTitle").textContent = saved + "'s To-Do List";
        }
        return;
    }
    var name = prompt("Enter your name for the page title (or leave blank):");
    if (name === null) {
        localStorage.setItem(NAME_KEY, "");
        return;
    }
    name = name.trim();
    localStorage.setItem(NAME_KEY, name);
    if (name.length > 0) {
        document.getElementById("pageTitle").textContent = name + "'s To-Do List";
    }
}

function setWelcomeLine() {
    var p = document.getElementById("welcomeLine");
    if (p) {
        p.textContent = "Welcome! Today is " + new Date().toLocaleDateString() + ".";
    }
}

// Email validation
function validateEmail() {
    var email = document.getElementById("email").value;

    var atpos = email.indexOf("@");
    var dotpos = email.lastIndexOf(".");

    if (atpos < 1 || dotpos - atpos < 2) {
        alert("Please enter correct email ID");
        return;
    }

    var pattern = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

    if (!pattern.test(email)) {
        alert("Invalid email!");
        return;
    }

    alert("Email saved successfully!");
}

// Add task
function addTask() {
    var task = prompt("Enter task:");
    var priority = prompt("Enter priority (number):");

    if (task === null || task.trim() === "") {
        alert("Task cannot be empty!");
        return;
    }

    if (priority === null || isNaN(priority) || String(priority).trim() === "") {
        alert("Priority must be a number!");
        return;
    }

    tasks.push({
        text: task.trim(),
        priority: String(priority).trim(),
        done: false
    });

    saveTasks();
    renderTasks();
}

// Clear all
function clearAll() {
    if (tasks.length === 0) {
        alert("No tasks to clear!");
        return;
    }

    if (confirm("Delete all tasks?")) {
        tasks = [];
        saveTasks();
        renderTasks();
    }
}

function initPage() {
    maybeAskName();
    setWelcomeLine();
    loadTasksFromStorage();
    renderTasks();
    updateTaskCount();
    if (tasks.length === 0 && !localStorage.getItem(TIME_KEY)) {
        var st = document.getElementById("statusLine");
        if (st) {
            st.textContent = "No tasks saved yet";
        }
    } else {
        updateStatusLine();
    }
}

initPage();
