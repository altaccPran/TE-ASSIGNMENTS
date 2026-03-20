import streamlit as st


def render_hero():
    st.markdown('<div id="hero"></div>', unsafe_allow_html=True)

    left, right = st.columns([1.15, 1], gap="large")

    with left:
        st.markdown("""
        <div class="hero-text">
            <div class="hero-badge">✦ Trusted by 10,000+ merchants in India</div>
            <h1 class="hero-headline">
                Build Your<br>
                <span class="hero-accent">Dream Store</span><br>
                In Minutes.
            </h1>
            <p class="hero-sub">
                StoreForge is the fastest way to launch, manage and grow
                your online store — no coding, no complexity, no excuses.
            </p>
            <div class="hero-buttons">
                <a href="#pricing" class="btn-primary">Start for Free →</a>
                <a href="#how-it-works" class="btn-ghost">See How It Works</a>
            </div>
            <p class="hero-footnote">✓ No credit card &nbsp;&nbsp; ✓ Live in under 5 minutes &nbsp;&nbsp; ✓ Cancel anytime</p>
        </div>
        """, unsafe_allow_html=True)

    with right:
        st.markdown("""
        <div class="hero-mockup">
            <div class="mockup-bar">
                <span class="dot red"></span>
                <span class="dot yellow"></span>
                <span class="dot green"></span>
                <span class="mockup-url">yourstore.storeforge.in</span>
            </div>
            <div class="mockup-nav">
                <span class="mockup-logo">✦ My Store</span>
                <span class="mockup-navlinks">Home &nbsp; Shop &nbsp; About</span>
            </div>
            <div class="mockup-hero-banner">
                <div class="mockup-banner-text">Summer Collection 2025</div>
                <div class="mockup-banner-sub">Shop the latest arrivals</div>
                <div class="mockup-banner-btn">Shop Now</div>
            </div>
            <div class="mockup-grid">
                <div class="mockup-card">
                    <div class="mockup-img"></div>
                    <div class="mockup-card-name">Product Name</div>
                    <div class="mockup-card-price">₹1,299</div>
                    <div class="mockup-card-btn">Add to Cart</div>
                </div>
                <div class="mockup-card">
                    <div class="mockup-img accent"></div>
                    <div class="mockup-card-name">Product Name</div>
                    <div class="mockup-card-price">₹899</div>
                    <div class="mockup-card-btn">Add to Cart</div>
                </div>
                <div class="mockup-card">
                    <div class="mockup-img soft"></div>
                    <div class="mockup-card-name">Product Name</div>
                    <div class="mockup-card-price">₹2,199</div>
                    <div class="mockup-card-btn">Add to Cart</div>
                </div>
            </div>
        </div>
        """, unsafe_allow_html=True)
