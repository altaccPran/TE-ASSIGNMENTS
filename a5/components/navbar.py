import streamlit as st


def render_navbar():
    st.markdown("""
    <nav class="navbar">
        <div class="nav-logo">⚡ StoreForge</div>
        <div class="nav-links">
            <a href="#features">Features</a>
            <a href="#how-it-works">How It Works</a>
            <a href="#pricing">Pricing</a>
            <a href="#testimonials">Reviews</a>
        </div>
        <a href="#pricing" class="nav-cta">Start for Free →</a>
    </nav>
    """, unsafe_allow_html=True)
