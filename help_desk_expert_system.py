class HelpDeskExpertSystem:
    def __init__(self):
        # 1. The Knowledge Base (Rules)
        # Each rule has a diagnosis/solution and a list of symptoms that trigger it.
        self.rules = [
            {
                "solution": "Action: Restart your router. Unplug it for 30 seconds, then plug it back in.",
                "symptoms": ["cannot_browse", "router_lights_abnormal"]
            },
            {
                "solution": "Action: Contact your ISP. There appears to be a broader network outage in your area.",
                "symptoms": ["cannot_browse", "isp_outage_reported"]
            },
            {
                "solution": "Action: Check your power source. Ensure the PC is plugged directly into a working wall outlet.",
                "symptoms": ["pc_dead", "no_fan_noise", "no_lights"]
            },
            {
                "solution": "Action: Your monitor might be disconnected or dead. Check the HDMI/DisplayPort cable.",
                "symptoms": ["pc_dead", "has_fan_noise", "screen_black"]
            },
            {
                "solution": "Action: Clear the print queue and restart the Print Spooler service.",
                "symptoms": ["printer_fails", "printer_status_offline"]
            },
            {
                "solution": "Action: Open the printer tray and carefully remove any jammed paper.",
                "symptoms": ["printer_fails", "printer_paper_error"]
            },
            {
                "solution": "Action: Reset your password using the Self-Service Password Portal.",
                "symptoms": ["login_fails", "account_locked"]
            }
        ]

        # Natural language mapping for the symptoms to ask the user
        self.questions = {
            "cannot_browse": "Are you unable to access any websites on the internet?",
            "router_lights_abnormal": "Are the lights on your router flashing red or completely off?",
            "isp_outage_reported": "Have your neighbors or colleagues also lost internet access?",
            "pc_dead": "Is your computer failing to turn on or show a display?",
            "no_fan_noise": "Is the computer completely silent (no fan noises)?",
            "no_lights": "Are all the indicator lights on the computer case off?",
            "has_fan_noise": "Can you hear the computer fans spinning?",
            "screen_black": "Is the monitor screen completely black?",
            "printer_fails": "Are you trying to print a document but nothing is happening?",
            "printer_status_offline": "Does your computer show the printer status as 'Offline'?",
            "printer_paper_error": "Is there a red blinking light or paper jam error on the printer?",
            "login_fails": "Are you unable to log into your company account?",
            "account_locked": "Does the screen say 'Account Locked' or 'Too many attempts'?"
        }

        # 2. Working Memory (User Responses)
        self.memory = {}

    def ask_question(self, symptom_id):
        """Asks the user a question if it hasn't been asked yet."""
        # If we already know the answer, don't ask again
        if symptom_id in self.memory:
            return self.memory[symptom_id]

        # Format CLI input
        print(f"\n[?] {self.questions[symptom_id]}")
        while True:
            response = input("    Reply (y/n): ").strip().lower()
            if response in ['y', 'yes']:
                self.memory[symptom_id] = True
                return True
            elif response in ['n', 'no']:
                self.memory[symptom_id] = False
                return False
            else:
                print("    Invalid input. Please enter 'y' or 'n'.")

    def run(self):
        """3. The Inference Engine"""
        print("========================================")
        print("   IT HELP DESK EXPERT SYSTEM v1.0")
        print("========================================")
        print("Please answer the following questions to diagnose your issue.\n")

        # Iterate through rules to find a match
        for rule in self.rules:
            match = True
            for symptom in rule["symptoms"]:
                # If any symptom is False, this rule is invalid. Break and check the next rule.
                if not self.ask_question(symptom):
                    match = False
                    break
            
            # If all symptoms for a rule returned True, we found our diagnosis!
            if match:
                print("\n" + "="*40)
                print(" DIAGNOSIS COMPLETE ")
                print("="*40)
                print(rule["solution"])
                print("========================================\n")
                return

        # If we loop through all rules and nothing matches perfectly
        print("\n" + "="*40)
        print(" DIAGNOSIS INCONCLUSIVE ")
        print("="*40)
        print("Action: Your issue requires human intervention. Escalating to Tier 2 Support.")
        print("Please call the Help Desk at 555-0199.")
        print("========================================\n")

if __name__ == "__main__":
    system = HelpDeskExpertSystem()
    system.run()