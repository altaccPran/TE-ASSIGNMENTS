# Assignment 3: XML, DTD, XSD, CSS, and XSLT

This folder implements a small **employee roster** as XML, constrains it with a **DTD** and **XML Schema**, and demonstrates two ways to **view** the data: **CSS** on raw XML (structured blocks) and **XSLT** to a responsive **HTML table**.

## What was implemented

| File | Role |
|------|------|
| `employees.xml` | Well-formed instance document: at least five `<employee>` records with Employee ID (`employeeId` attribute), first/last name, department, designation, salary, and email. Links to the DTD, XSD, `style.css`, and `transform.xsl` via the DOCTYPE, `xsi:noNamespaceSchemaLocation`, and `xml-stylesheet` processing instructions. |
| `employees.dtd` | Document Type Definition: legal element order, required `employeeId` (typed as `ID` in the DTD), and optional schema-instance attributes on the root so validation works with `xsi:*` hints. |
| `employees.xsd` | XML Schema: stronger rules than the DTD (e.g. `xs:decimal` salary, `xs:ID` for `employeeId`, email pattern). |
| `style.css` | Styles **raw** XML in the browser as labeled, card-style blocks (not an HTML `<table>`; that requires XSLT). |
| `transform.xsl` | XSLT 1.0 stylesheet that transforms `employees.xml` into a full HTML page with a `<table>`, `<thead>` / `<tbody>`, and internal CSS for layout and horizontal scrolling on small screens. |

**Important:** In `employees.xml`, keep **only one** of the two `<?xml-stylesheet …?>` lines active at a time (comment the other). Using both can make browser behavior unpredictable.

## How to demonstrate in a web browser

All five files must live in the **same directory**. Paths in `employees.xml` assume that (`style.css`, `transform.xsl`, `employees.dtd`, `employees.xsd`).

### Option A — CSS-styled XML

1. In `employees.xml`, **uncomment** the line with `href="style.css"` and **comment out** the line with `href="transform.xsl"`.
2. Save the file.
3. Open `employees.xml` in **Mozilla Firefox** via **File → Open File…** (or drag the file into a window).

Firefox is recommended: it applies CSS to arbitrary XML reliably. Other browsers may show an unstyled tree or limited styling.

### Option B — XSLT → HTML table

1. **Uncomment** the line with `href="transform.xsl"` and **comment out** the CSS `xml-stylesheet` line.
2. Save the file.
3. Open `employees.xml` in **Firefox** as above. You should see a titled page with a data table, not the raw XML.

If transformation does not run from `file://` (browser security varies), serve this folder over HTTP and open the same URL:

```bash
cd /path/to/TE-ASSIGNMENTS/a4
python3 -m http.server 8000
```

Then visit `http://localhost:8000/employees.xml` in Firefox.

## How to demonstrate DTD and XSD validation

Browsers do **not** usually report full DTD/XSD validation in the page UI. Use a validating tool (e.g. **xmllint**, which is part of **libxml2** on many systems).

From this directory:

```bash
# DTD validation
xmllint --noout --dtdvalid employees.dtd employees.xml

# XML Schema validation
xmllint --noout --schema employees.xsd employees.xml
```

A successful run prints nothing and exits with status 0. To show that rules matter, temporarily break the XML (e.g. remove an element or set `salary` to non-numeric text) and run the same commands again; validation should fail.

## Data shape (quick reference)

- Root: `<employees>`.
- Each child: `<employee employeeId="…">` with children in order: `firstName`, `lastName`, `department`, `designation`, `salary`, `email`.

This matches the constraints declared in `employees.dtd` and `employees.xsd`.
