<?php
/**
 * Employee Directory — single-page CRUD (Create, Read, Update, Delete)
 * Uses MySQLi prepared statements (no raw string SQL with user input).
 */

declare(strict_types=1);

require __DIR__ . '/db_connect.php';

/** HTML escape helper */
function h(?string $s): string
{
    return htmlspecialchars((string) $s, ENT_QUOTES, 'UTF-8');
}

// --- Handle POST (Create / Update / Delete) then redirect (PRG) ---
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['delete_id'])) {
        $id = (int) $_POST['delete_id'];
        if ($id > 0) {
            $stmt = $mysqli->prepare('DELETE FROM employees WHERE id = ? LIMIT 1');
            $stmt->bind_param('i', $id);
            $stmt->execute();
            $stmt->close();
        }
        header('Location: index.php?msg=deleted');
        exit;
    }

    $name = trim((string) ($_POST['name'] ?? ''));
    $email = trim((string) ($_POST['email'] ?? ''));
    $department = trim((string) ($_POST['department'] ?? ''));
    $designation = trim((string) ($_POST['designation'] ?? ''));
    $editId = isset($_POST['id']) ? (int) $_POST['id'] : 0;

    if ($name === '' || $email === '' || $department === '' || $designation === '') {
        header('Location: index.php?msg=invalid');
        exit;
    }

    if ($editId > 0) {
        $stmt = $mysqli->prepare(
            'UPDATE employees SET name = ?, email = ?, department = ?, designation = ? WHERE id = ? LIMIT 1'
        );
        $stmt->bind_param('ssssi', $name, $email, $department, $designation, $editId);
        $stmt->execute();
        $stmt->close();
        header('Location: index.php?msg=updated');
        exit;
    }

    $stmt = $mysqli->prepare(
        'INSERT INTO employees (name, email, department, designation) VALUES (?, ?, ?, ?)'
    );
    $stmt->bind_param('ssss', $name, $email, $department, $designation);
    try {
        $stmt->execute();
    } catch (mysqli_sql_exception $e) {
        // e.g. duplicate email (unique constraint)
        header('Location: index.php?msg=duplicate');
        exit;
    }
    $stmt->close();
    header('Location: index.php?msg=added');
    exit;
}

// --- Load row for edit (GET ?edit=id) ---
$editRow = null;
if (isset($_GET['edit'])) {
    $eid = (int) $_GET['edit'];
    if ($eid > 0) {
        $stmt = $mysqli->prepare('SELECT id, name, email, department, designation FROM employees WHERE id = ? LIMIT 1');
        $stmt->bind_param('i', $eid);
        $stmt->execute();
        $res = $stmt->get_result();
        $editRow = $res->fetch_assoc();
        $stmt->close();
    }
}

// --- Read all employees ---
$rows = [];
$res = $mysqli->query('SELECT id, name, email, department, designation FROM employees ORDER BY id ASC');
if ($res) {
    while ($row = $res->fetch_assoc()) {
        $rows[] = $row;
    }
    $res->free();
}

$mysqli->close();

$msg = $_GET['msg'] ?? '';
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Employee Directory</title>
    <style>
        body { font-family: system-ui, Segoe UI, sans-serif; margin: 0; background: #f4f6f8; color: #222; }
        .wrap { max-width: 960px; margin: 0 auto; padding: 1.25rem; }
        h1 { font-size: 1.35rem; margin: 0 0 0.75rem; }
        .banner { padding: 0.65rem 0.85rem; border-radius: 6px; margin-bottom: 1rem; font-size: 0.95rem; }
        .banner.ok { background: #e6f4ea; border: 1px solid #34a853; color: #1e4620; }
        .banner.err { background: #fce8e6; border: 1px solid #ea4335; color: #5f1e1a; }
        .grid { display: grid; grid-template-columns: 1fr; gap: 1.25rem; }
        @media (min-width: 720px) { .grid { grid-template-columns: 1fr 1.4fr; } }
        .card { background: #fff; border: 1px solid #ddd; border-radius: 8px; padding: 1rem; box-shadow: 0 1px 2px rgba(0,0,0,0.06); }
        label { display: block; font-size: 0.85rem; font-weight: 600; margin: 0.5rem 0 0.25rem; }
        input[type="text"], input[type="email"] { width: 100%; max-width: 100%; padding: 0.45rem 0.5rem; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }
        .actions { margin-top: 0.75rem; display: flex; gap: 0.5rem; flex-wrap: wrap; align-items: center; }
        button, .btn-link { cursor: pointer; font-size: 0.9rem; padding: 0.45rem 0.75rem; border-radius: 4px; border: 1px solid #ccc; background: #f8f9fa; text-decoration: none; color: #111; display: inline-block; }
        button.primary { background: #1a73e8; border-color: #1a73e8; color: #fff; }
        button.danger { background: #fff; border-color: #c5221f; color: #c5221f; }
        table { width: 100%; border-collapse: collapse; font-size: 0.9rem; }
        th, td { border: 1px solid #e0e0e0; padding: 0.5rem 0.6rem; text-align: left; vertical-align: top; }
        th { background: #eef1f4; }
        tr:nth-child(even) { background: #fafafa; }
        .small { font-size: 0.85rem; color: #555; margin: 0 0 0.75rem; }
        .nowrap { white-space: nowrap; }
    </style>
</head>
<body>
<div class="wrap">
    <h1>Employee Directory</h1>
    <p class="small">PHP + MySQL (MySQLi prepared statements). Database: <code>company_db</code>, table: <code>employees</code>.</p>

    <?php if ($msg === 'added'): ?>
        <div class="banner ok">Employee added.</div>
    <?php elseif ($msg === 'updated'): ?>
        <div class="banner ok">Employee updated.</div>
    <?php elseif ($msg === 'deleted'): ?>
        <div class="banner ok">Employee deleted.</div>
    <?php elseif ($msg === 'invalid'): ?>
        <div class="banner err">Please fill in all fields.</div>
    <?php elseif ($msg === 'duplicate'): ?>
        <div class="banner err">Could not save (email may already exist).</div>
    <?php endif; ?>

    <div class="grid">
        <div class="card">
            <h2 style="margin:0 0 0.5rem; font-size:1.05rem;"><?php echo $editRow ? 'Edit employee' : 'Add employee'; ?></h2>
            <form method="post" action="index.php" autocomplete="off">
                <?php if ($editRow): ?>
                    <input type="hidden" name="id" value="<?php echo (int) $editRow['id']; ?>">
                <?php endif; ?>
                <label for="name">Name</label>
                <input type="text" id="name" name="name" required maxlength="120"
                       value="<?php echo h($editRow['name'] ?? ''); ?>">

                <label for="email">Email</label>
                <input type="email" id="email" name="email" required maxlength="180"
                       value="<?php echo h($editRow['email'] ?? ''); ?>">

                <label for="department">Department</label>
                <input type="text" id="department" name="department" required maxlength="100"
                       value="<?php echo h($editRow['department'] ?? ''); ?>">

                <label for="designation">Designation</label>
                <input type="text" id="designation" name="designation" required maxlength="100"
                       value="<?php echo h($editRow['designation'] ?? ''); ?>">

                <div class="actions">
                    <button type="submit" name="save" class="primary"><?php echo $editRow ? 'Update' : 'Add'; ?></button>
                    <?php if ($editRow): ?>
                        <a class="btn-link" href="index.php">Cancel edit</a>
                    <?php endif; ?>
                </div>
            </form>
        </div>

        <div class="card">
            <h2 style="margin:0 0 0.5rem; font-size:1.05rem;">All employees</h2>
            <?php if (count($rows) === 0): ?>
                <p class="small">No employees yet. Add one using the form.</p>
            <?php else: ?>
                <div style="overflow-x:auto;">
                    <table>
                        <thead>
                            <tr>
                                <th>ID</th>
                                <th>Name</th>
                                <th>Email</th>
                                <th>Department</th>
                                <th>Designation</th>
                                <th class="nowrap">Actions</th>
                            </tr>
                        </thead>
                        <tbody>
                        <?php foreach ($rows as $r): ?>
                            <tr>
                                <td><?php echo (int) $r['id']; ?></td>
                                <td><?php echo h($r['name']); ?></td>
                                <td><?php echo h($r['email']); ?></td>
                                <td><?php echo h($r['department']); ?></td>
                                <td><?php echo h($r['designation']); ?></td>
                                <td class="nowrap">
                                    <a class="btn-link" href="index.php?edit=<?php echo (int) $r['id']; ?>">Edit</a>
                                    <form method="post" action="index.php" style="display:inline;" onsubmit="return confirm('Delete this employee?');">
                                        <input type="hidden" name="delete_id" value="<?php echo (int) $r['id']; ?>">
                                        <button type="submit" class="danger">Delete</button>
                                    </form>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                        </tbody>
                    </table>
                </div>
            <?php endif; ?>
        </div>
    </div>
</div>
</body>
</html>
