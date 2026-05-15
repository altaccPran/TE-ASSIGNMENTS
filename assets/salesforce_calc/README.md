# Native Salesforce Scientific Calculator

This README walks through building a native Apex + Visualforce scientific calculator in Salesforce: backend controller, Visualforce page, tab setup, and how to run it.

**Note:** The Apex class and Visualforce page must be created in Salesforce (Developer Console or your IDE with Salesforce extensions). This file is documentation only.

---

## Step 1: Create the Apex Backend (The Controller)

Create an Apex class that holds your variables and performs the scientific calculations.

1. Click the **Gear** icon (top right) in Salesforce and select **Developer Console** (opens in a new window).
2. In Developer Console: **File → New → Apex Class**.
3. Name the class **`NativeCalculatorController`** and click **OK**.
4. Paste the following Apex (getter/setter variables pass data between Apex and the Visualforce screen):

```apex
public class NativeCalculatorController {
    // Variables bound to the frontend inputs and outputs
    public Double operand1 { get; set; }
    public Double operand2 { get; set; }
    public Double result { get; set; }

    // --- Basic Operations ---
    public void add() {
        if (operand1 != null && operand2 != null) result = operand1 + operand2;
    }

    public void subtract() {
        if (operand1 != null && operand2 != null) result = operand1 - operand2;
    }

    public void multiply() {
        if (operand1 != null && operand2 != null) result = operand1 * operand2;
    }

    public void divide() {
        if (operand1 != null && operand2 != null && operand2 != 0) {
            result = operand1 / operand2;
        } else {
            ApexPages.addMessage(new ApexPages.Message(ApexPages.Severity.ERROR, 'Cannot divide by zero.'));
        }
    }

    // --- Scientific Operations ---
    public void calculatePower() {
        if (operand1 != null && operand2 != null) {
            result = Math.pow(operand1, operand2);
        }
    }

    public void calculateSquareRoot() {
        if (operand1 != null && operand1 >= 0) {
            result = Math.sqrt(operand1);
        } else {
            ApexPages.addMessage(new ApexPages.Message(ApexPages.Severity.ERROR, 'Invalid input for square root.'));
        }
    }

    public void calculateLog10() {
        if (operand1 != null && operand1 > 0) {
            result = Math.log10(operand1);
        }
    }

    public void clear() {
        operand1 = null;
        operand2 = null;
        result = null;
    }
}
```

5. **File → Save** (or **Ctrl+S** / **Cmd+S**).

---

## Step 2: Create the Frontend (Visualforce Page)

1. Still in Developer Console: **File → New → Visualforce Page**.
2. Name the page **`NativeCalculatorPage`** and click **OK**.
3. Paste the following markup. The `controller` attribute links to your Apex class; `{!…}` binds inputs to Apex variables.

```xml
<apex:page controller="NativeCalculatorController" docType="html-5.0" lightningStylesheets="true">
    <apex:form >
        <apex:pageMessages id="messages"/>

        <apex:pageBlock title="Apex Scientific Calculator">
            <apex:pageBlockSection columns="1">
                <apex:input type="number" value="{!operand1}" label="Value 1 (Base/Main Value)" />
                <apex:input type="number" value="{!operand2}" label="Value 2 (Exponent/Secondary Value)" />
            </apex:pageBlockSection>

            <apex:pageBlockSection columns="1">
                <apex:outputText value="Result: {!result}" style="font-size: 18px; font-weight: bold; color: #1589ee;" id="calcResult"/>
            </apex:pageBlockSection>

            <apex:pageBlockButtons location="bottom">
                <apex:commandButton value="+" action="{!add}" />
                <apex:commandButton value="-" action="{!subtract}" />
                <apex:commandButton value="*" action="{!multiply}" />
                <apex:commandButton value="/" action="{!divide}" />
                <apex:commandButton value="x^y" action="{!calculatePower}" />
                <apex:commandButton value="√x" action="{!calculateSquareRoot}" title="Uses Value 1 only" />
                <apex:commandButton value="log10(x)" action="{!calculateLog10}" title="Uses Value 1 only" />
                <apex:commandButton value="Clear" action="{!clear}" />
            </apex:pageBlockButtons>
        </apex:pageBlock>
    </apex:form>
</apex:page>
```

`lightningStylesheets="true"` tells Visualforce to mimic the modern Salesforce look without extra CSS.

4. **File → Save**.

---

## Step 3: Turn the Page into a Salesforce “App” (Tab)

Expose the page via a tab so it appears like a normal app item.

1. Return to the main Salesforce window (you can leave Developer Console open).
2. **Setup** (Gear → **Setup**).
3. Quick Find: type **Tabs** → open **Tabs**.
4. Scroll to **Visualforce Tabs** → **New**.
5. **Step 3.1:** Visualforce Page: **`NativeCalculatorPage`**. Set **Label** (e.g. **Scientific Calculator**) and **Tab Name**. Pick a **Tab Style** → **Next**.
6. **Step 3.2:** Leave default profile visibility if desired (**Apply one tab visibility to all profiles**) → **Next**.
7. **Step 3.3:** Choose which standard apps include this tab (e.g. Sales, Service) or leave all checked → **Save**.

---

## Step 4: Run Your Calculator

1. Open the **App Launcher** (9-dot grid, top left).
2. Search for **Scientific Calculator** (or your tab label) and open the tab.
3. Enter values and use the operation buttons; the page posts back and shows results from Apex.

---

## Quick reference

| Artifact | Name |
|----------|------|
| Apex class | `NativeCalculatorController` |
| Visualforce page | `NativeCalculatorPage` |
| Suggested tab label | Scientific Calculator |
