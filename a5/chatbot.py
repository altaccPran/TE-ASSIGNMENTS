import streamlit as st
from data import FAQS


def get_response(user_input: str) -> str:
    msg = user_input.lower().strip()

    # Direct keyword scan
    for keyword, answer in FAQS.items():
        if keyword in msg:
            return answer

    # Fallback phrase matching
    if any(w in msg for w in ["hello", "hi", "hey", "hlo"]):
        return "Hey there! 👋 I can help with questions about pricing, features, themes, domains, chatbot, support, or anything else about our platform."
    if any(w in msg for w in ["thank", "thanks", "thx"]):
        return "You're welcome! Feel free to ask if you have more questions. 😊"
    if any(w in msg for w in ["bye", "goodbye", "see you"]):
        return "Goodbye! Come back anytime. Good luck with your store! 🚀"
    if any(w in msg for w in ["help", "assist", "support"]):
        return FAQS["support"]
    if any(w in msg for w in ["cost", "price", "plan", "subscription", "how much"]):
        return FAQS["pricing"]
    if any(w in msg for w in ["product", "item", "catalogue", "catalog", "inventory"]):
        return FAQS["products"]
    if any(w in msg for w in ["secure", "ssl", "safe", "hack", "encrypt"]):
        return FAQS["security"]
    if any(w in msg for w in ["phone", "tablet", "responsive"]):
        return FAQS["mobile"]
    if any(w in msg for w in ["shopify", "woocommerce", "migrate", "import", "switch"]):
        return FAQS["migration"]

    return (
        "I'm not sure about that one! Try asking about:\n\n"
        "• **Pricing & plans** — what each plan includes\n"
        "• **Products** — how many you can add\n"
        "• **Themes** — customising your store look\n"
        "• **Domains** — connecting your own domain\n"
        "• **Chatbot** — the AI assistant feature\n"
        "• **Support** — how to get help"
    )


def render_chatbot():
    st.sidebar.markdown("""
    <div style='padding: 0.5rem 0 0.25rem 0;'>
        <span style='font-size:1.15rem; font-weight:700; letter-spacing:-0.5px;'>🤖 Store Assistant</span><br>
        <span style='font-size:0.75rem; color:#888;'>Ask me anything about the platform</span>
    </div>
    """, unsafe_allow_html=True)

    st.sidebar.divider()

    if "chat_history" not in st.session_state:
        st.session_state.chat_history = [
            {"role": "assistant", "content": "Hi! I'm your store assistant 👋\n\nAsk me about pricing, features, themes, domains, or anything else!"}
        ]

    # Render chat history
    for msg in st.session_state.chat_history:
        with st.sidebar.chat_message(msg["role"]):
            st.markdown(msg["content"])

    # Chat input
    if prompt := st.sidebar.chat_input("Ask a question..."):
        st.session_state.chat_history.append({"role": "user", "content": prompt})
        response = get_response(prompt)
        st.session_state.chat_history.append({"role": "assistant", "content": response})
        st.rerun()
