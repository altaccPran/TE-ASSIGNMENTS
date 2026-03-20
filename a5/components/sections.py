import streamlit as st
from data import STATS, FEATURES, STEPS, PRICING, TESTIMONIALS, FOOTER_LINKS


# ── Stats Bar ──────────────────────────────────────────────────────────────
def render_stats():
    st.markdown('<div class="section-divider"></div>', unsafe_allow_html=True)
    cols = st.columns(4)
    for col, stat in zip(cols, STATS):
        with col:
            st.markdown(f"""
            <div class="stat-card">
                <div class="stat-value">{stat['value']}</div>
                <div class="stat-label">{stat['label']}</div>
            </div>
            """, unsafe_allow_html=True)
    st.markdown('<div class="section-divider"></div>', unsafe_allow_html=True)


# ── Features ───────────────────────────────────────────────────────────────
def render_features():
    st.markdown('<div id="features"></div>', unsafe_allow_html=True)
    st.markdown("""
    <div class="section-header">
        <div class="section-tag">Features</div>
        <h2 class="section-title">Everything you need.<br>Nothing you don't.</h2>
        <p class="section-sub">One platform, every tool — from product listings to AI-powered sales.</p>
    </div>
    """, unsafe_allow_html=True)

    row1 = st.columns(3, gap="medium")
    row2 = st.columns(3, gap="medium")

    for i, (col, feat) in enumerate(zip(row1 + row2, FEATURES)):
        with col:
            st.markdown(f"""
            <div class="feature-card">
                <div class="feature-icon">{feat['icon']}</div>
                <div class="feature-title">{feat['title']}</div>
                <div class="feature-desc">{feat['desc']}</div>
            </div>
            """, unsafe_allow_html=True)


# ── How It Works ───────────────────────────────────────────────────────────
def render_how_it_works():
    st.markdown('<div id="how-it-works"></div>', unsafe_allow_html=True)
    st.markdown('<div class="section-divider"></div>', unsafe_allow_html=True)
    st.markdown("""
    <div class="section-header dark-bg">
        <div class="section-tag inverted">How It Works</div>
        <h2 class="section-title inverted">From zero to live store<br>in three steps.</h2>
    </div>
    """, unsafe_allow_html=True)

    cols = st.columns(3, gap="large")
    for i, (col, step) in enumerate(zip(cols, STEPS)):
        with col:
            connector = "step-connector" if i < 2 else ""
            st.markdown(f"""
            <div class="step-card {connector}">
                <div class="step-number">{step['number']}</div>
                <div class="step-title">{step['title']}</div>
                <div class="step-desc">{step['desc']}</div>
            </div>
            """, unsafe_allow_html=True)

    st.markdown('<div class="section-divider"></div>', unsafe_allow_html=True)


# ── Pricing ────────────────────────────────────────────────────────────────
def render_pricing():
    st.markdown('<div id="pricing"></div>', unsafe_allow_html=True)
    st.markdown("""
    <div class="section-header">
        <div class="section-tag">Pricing</div>
        <h2 class="section-title">Simple, honest pricing.</h2>
        <p class="section-sub">Start free. Scale when you're ready. No hidden fees, ever.</p>
    </div>
    """, unsafe_allow_html=True)

    cols = st.columns(3, gap="medium")
    for col, plan in zip(cols, PRICING):
        with col:
            popular_badge = '<div class="popular-badge">Most Popular</div>' if plan["popular"] else '<div class="popular-badge invisible">.</div>'
            card_class = "pricing-card popular" if plan["popular"] else "pricing-card"

            features_html = ""
            for check, feat in plan["features"]:
                tick_class = "tick-yes" if check == "✓" else "tick-no"
                features_html += f'<div class="plan-feature"><span class="{tick_class}">{check}</span> {feat}</div>'

            st.markdown(f"""
            {popular_badge}
            <div class="{card_class}">
                <div class="plan-name">{plan['name']}</div>
                <div class="plan-price">
                    {plan['price']}<span class="plan-period">{plan['period']}</span>
                </div>
                <div class="plan-divider"></div>
                {features_html}
                <a href="#" class="plan-cta" onclick="return false;">{plan['cta']}</a>
            </div>
            """, unsafe_allow_html=True)


# ── Testimonials ───────────────────────────────────────────────────────────
def render_testimonials():
    st.markdown('<div id="testimonials"></div>', unsafe_allow_html=True)
    st.markdown('<div class="section-divider"></div>', unsafe_allow_html=True)
    st.markdown("""
    <div class="section-header">
        <div class="section-tag">Reviews</div>
        <h2 class="section-title">Merchants love us.</h2>
        <p class="section-sub">Don't take our word for it — here's what real store owners say.</p>
    </div>
    """, unsafe_allow_html=True)

    cols = st.columns(3, gap="medium")
    for col, t in zip(cols, TESTIMONIALS):
        with col:
            st.markdown(f"""
            <div class="testimonial-card">
                <div class="quote-mark">"</div>
                <p class="quote-text">{t['quote']}</p>
                <div class="quote-author">
                    <div class="author-avatar">{t['name'][0]}</div>
                    <div>
                        <div class="author-name">{t['name']}</div>
                        <div class="author-store">{t['store']}</div>
                    </div>
                </div>
            </div>
            """, unsafe_allow_html=True)


# ── CTA Banner ─────────────────────────────────────────────────────────────
def render_cta():
    st.markdown('<div class="section-divider"></div>', unsafe_allow_html=True)
    st.markdown("""
    <div class="cta-banner">
        <div class="cta-tag">✦ Limited time — first 3 months free on Growth</div>
        <h2 class="cta-title">Ready to launch your store?</h2>
        <p class="cta-sub">Join 10,000+ merchants already selling on StoreForge.</p>
        <div class="cta-buttons">
            <a href="#pricing" class="btn-primary large">Start for Free →</a>
            <a href="#features" class="btn-ghost light large">Explore Features</a>
        </div>
    </div>
    """, unsafe_allow_html=True)


# ── Footer ─────────────────────────────────────────────────────────────────
def render_footer():
    st.markdown('<div class="section-divider"></div>', unsafe_allow_html=True)

    cols = st.columns([1.5, 1, 1, 1], gap="large")

    with cols[0]:
        st.markdown("""
        <div class="footer-brand">
            <div class="footer-logo">⚡ StoreForge</div>
            <p class="footer-tagline">The fastest way to launch<br>and grow your online store.</p>
            <div class="footer-social">
                <span>𝕏</span><span>in</span><span>▶</span>
            </div>
        </div>
        """, unsafe_allow_html=True)

    for col, (heading, links) in zip(cols[1:], FOOTER_LINKS.items()):
        with col:
            links_html = "".join(f'<a class="footer-link" href="#">{l}</a>' for l in links)
            st.markdown(f"""
            <div class="footer-col">
                <div class="footer-col-heading">{heading}</div>
                {links_html}
            </div>
            """, unsafe_allow_html=True)

    st.markdown("""
    <div class="footer-bottom">
        <span>© 2025 StoreForge Technologies Pvt. Ltd. All rights reserved.</span>
        <span>
            <a href="#">Privacy Policy</a> &nbsp;·&nbsp;
            <a href="#">Terms of Service</a> &nbsp;·&nbsp;
            <a href="#">Cookie Policy</a>
        </span>
    </div>
    """, unsafe_allow_html=True)
