function addTask() {
    let task = prompt("Enter a new task:");

    // Validation
    if (task === null || task.trim() === "") {
        alert("Task cannot be empty!");
        return;
    }

    let li = document.createElement("li");
    li.textContent = task;

    // Click to mark complete
    li.onclick = function () {
        if (li.style.textDecoration === "line-through") {
            li.style.textDecoration = "none";
        } else {
            li.style.textDecoration = "line-through";
        }
    };

    // Double click to delete
    li.ondblclick = function () {
        let confirmDelete = confirm("Delete this task?");
        if (confirmDelete) {
            li.remove();
        }
    };

    document.getElementById("taskList").appendChild(li);
}

function clearAll() {
    let list = document.getElementById("taskList");

    if (list.children.length === 0) {
        alert("No tasks to clear!");
        return;
    }

    let confirmClear = confirm("Are you sure you want to delete all tasks?");
    if (confirmClear) {
        list.innerHTML = "";
    }
}