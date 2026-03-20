FEATURES = [
    {"icon": "🏪", "title": "Store Builder",       "desc": "Launch your store in minutes — zero code, zero hassle."},
    {"icon": "📦", "title": "Product Management",  "desc": "Add, edit and organise unlimited products with ease."},
    {"icon": "🎨", "title": "Theme Picker",         "desc": "Choose from stunning ready-made themes and make it yours."},
    {"icon": "📊", "title": "Analytics Dashboard", "desc": "Track visits, revenue and your best-selling products live."},
    {"icon": "🌐", "title": "Custom Domain",        "desc": "Connect your own domain and go fully branded in one click."},
    {"icon": "🤖", "title": "AI Sales Chatbot",    "desc": "A built-in assistant that converts visitors into buyers 24/7."},
]

STEPS = [
    {"number": "01", "title": "Sign Up Free",      "desc": "Create your account in 30 seconds. No credit card needed."},
    {"number": "02", "title": "Build Your Store",  "desc": "Pick a theme, add your products, and customise every detail."},
    {"number": "03", "title": "Go Live",            "desc": "Hit publish and start selling to the world instantly."},
]

PRICING = [
    {
        "name": "Starter",
        "price": "Free",
        "period": "",
        "popular": False,
        "cta": "Get Started Free",
        "features": [
            ("✓", "1 store"),
            ("✓", "Up to 10 products"),
            ("✓", "Basic themes"),
            ("✓", "Community support"),
            ("✗", "Analytics"),
            ("✗", "Custom domain"),
            ("✗", "AI chatbot"),
        ],
    },
    {
        "name": "Growth",
        "price": "₹999",
        "period": "/month",
        "popular": True,
        "cta": "Start Growth Plan",
        "features": [
            ("✓", "3 stores"),
            ("✓", "Unlimited products"),
            ("✓", "All themes"),
            ("✓", "Priority support"),
            ("✓", "Analytics dashboard"),
            ("✓", "Custom domain"),
            ("✗", "AI chatbot"),
        ],
    },
    {
        "name": "Pro",
        "price": "₹2,499",
        "period": "/month",
        "popular": False,
        "cta": "Go Pro",
        "features": [
            ("✓", "Unlimited stores"),
            ("✓", "Unlimited products"),
            ("✓", "All themes"),
            ("✓", "Dedicated support"),
            ("✓", "Advanced analytics"),
            ("✓", "Custom domain"),
            ("✓", "AI sales chatbot"),
        ],
    },
]

TESTIMONIALS = [
    {
        "name":  "Priya Sharma",
        "store": "Priya's Boutique",
        "quote": "I launched my clothing store in a single afternoon. The theme picker is gorgeous and my customers love how it looks.",
    },
    {
        "name":  "Rahul Mehta",
        "store": "TechGadgets.in",
        "quote": "Managing 200+ products has never been this simple. The analytics alone are worth every rupee of the Growth plan.",
    },
    {
        "name":  "Anita Desai",
        "store": "Anita's Artisan Bakery",
        "quote": "The AI chatbot answers my customers at 2 AM when I'm asleep. It's like having a full-time sales assistant for free.",
    },
]

STATS = [
    {"value": "10,000+", "label": "Stores Launched"},
    {"value": "99.9%",   "label": "Uptime Guaranteed"},
    {"value": "4.9 ★",   "label": "Average Rating"},
    {"value": "₹50Cr+",  "label": "Merchant Revenue"},
]

FAQS = {
    "free":       "Yes! Our Starter plan is completely free — no credit card required, ever.",
    "pricing":    "Starter is free. Growth is ₹999/month and Pro is ₹2,499/month. Both paid plans include a 14-day money-back guarantee.",
    "products":   "The Starter plan supports up to 10 products. Growth and Pro plans have no product limits at all.",
    "theme":      "You can browse and switch themes anytime from your store dashboard. All themes are mobile-responsive.",
    "domain":     "Custom domains are available on the Growth and Pro plans. You can connect any domain you already own.",
    "chatbot":    "The AI sales chatbot is included in the Pro plan. It can answer customer questions, recommend products, and help close sales.",
    "support":    "Starter includes community forum support. Growth gets priority email support. Pro gets a dedicated account manager.",
    "cancel":     "You can cancel or downgrade your plan at any time — no lock-in contracts, no cancellation fees.",
    "refund":     "All paid plans come with a 14-day money-back guarantee. No questions asked.",
    "payment":    "We accept all major credit and debit cards, UPI, and net banking.",
    "mobile":     "Yes, all stores built on our platform are fully mobile-responsive out of the box.",
    "migration":  "We offer free store migration support on the Growth and Pro plans. Our team will help you move from Shopify or WooCommerce.",
    "trial":      "Growth and Pro plans come with a 14-day free trial. No credit card needed to start.",
    "security":   "All stores are SSL-secured by default. We are PCI-DSS compliant and your data is encrypted at rest.",
    "inventory":  "Yes, built-in inventory tracking is available on all plans. You'll get low-stock alerts automatically.",
}

FOOTER_LINKS = {
    "Product":  ["Features", "Pricing", "Themes", "Integrations", "Changelog"],
    "Support":  ["Help Centre", "Community", "Contact Us", "Status Page"],
    "Company":  ["About", "Blog", "Careers", "Press Kit"],
}
