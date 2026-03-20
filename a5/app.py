import streamlit as st
from chatbot import render_chatbot
from components.navbar import render_navbar
from components.hero import render_hero
from components.sections import (
    render_stats, render_features, render_how_it_works,
    render_pricing, render_testimonials, render_cta, render_footer
)

# ── Page config ────────────────────────────────────────────────────────────
st.set_page_config(
    page_title="StoreForge — Build Your Online Store",
    page_icon="⚡",
    layout="wide",
    initial_sidebar_state="expanded",
)

# ── Global CSS ─────────────────────────────────────────────────────────────
st.markdown("""
<style>
@import url('https://fonts.googleapis.com/css2?family=Sora:wght@300;400;500;600;700;800&family=DM+Sans:ital,wght@0,300;0,400;0,500;1,400&display=swap');

/* ── Reset & base ──────────────────────────── */
*, *::before, *::after { box-sizing: border-box; margin: 0; }

:root {
    --bg:       #0a0a0f;
    --bg2:      #111118;
    --bg3:      #18181f;
    --border:   rgba(255,255,255,0.08);
    --accent:   #f5a623;
    --accent2:  #ff6b6b;
    --text:     #f0eff5;
    --muted:    #8b8a99;
    --green:    #4ade80;
    --red:      #f87171;
}

html, body, [data-testid="stApp"] {
    background: var(--bg) !important;
    font-family: 'DM Sans', sans-serif;
    color: var(--text);
}

/* ── Hide Streamlit chrome ─────────────────── */
#MainMenu, footer, header,
[data-testid="stToolbar"],
[data-testid="stDecoration"],
[data-testid="stStatusWidget"]       { display: none !important; }
[data-testid="stSidebarCollapseButton"] { display: none !important; }
.stDeployButton                      { display: none !important; }

/* Remove top padding */
[data-testid="stAppViewContainer"] > .main { padding-top: 0 !important; }
.block-container { padding: 0 !important; max-width: 100% !important; }
[data-testid="column"] { padding: 0 0.5rem !important; }

/* ── Sidebar (chatbot) ────────────────────── */
[data-testid="stSidebar"] {
    background: var(--bg2) !important;
    border-right: 1px solid var(--border) !important;
}
[data-testid="stSidebar"] * { font-family: 'DM Sans', sans-serif !important; }

/* ── Navbar ────────────────────────────────── */
.navbar {
    display: flex; align-items: center; justify-content: space-between;
    padding: 1.1rem 3rem;
    background: rgba(10,10,15,0.95);
    backdrop-filter: blur(12px);
    border-bottom: 1px solid var(--border);
    position: sticky; top: 0; z-index: 999;
}
.nav-logo {
    font-family: 'Sora', sans-serif;
    font-size: 1.3rem; font-weight: 700;
    color: var(--accent); letter-spacing: -0.5px;
}
.nav-links { display: flex; gap: 2rem; }
.nav-links a {
    color: var(--muted); text-decoration: none;
    font-size: 0.9rem; font-weight: 500;
    transition: color 0.2s;
}
.nav-links a:hover { color: var(--text); }
.nav-cta {
    background: var(--accent); color: #0a0a0f;
    padding: 0.5rem 1.25rem; border-radius: 6px;
    font-weight: 700; font-size: 0.875rem;
    text-decoration: none; letter-spacing: -0.2px;
    transition: opacity 0.2s;
}
.nav-cta:hover { opacity: 0.88; }

/* ── Section utilities ────────────────────── */
.section-divider { height: 3rem; }
.section-header  { text-align: center; padding: 3rem 1rem 2rem; }
.section-tag {
    display: inline-block;
    background: rgba(245,166,35,0.12); color: var(--accent);
    font-size: 0.78rem; font-weight: 700; letter-spacing: 1.5px;
    text-transform: uppercase; padding: 0.3rem 0.9rem;
    border-radius: 100px; margin-bottom: 1rem;
    border: 1px solid rgba(245,166,35,0.25);
}
.section-tag.inverted { background: rgba(255,255,255,0.1); color: var(--text); border-color: rgba(255,255,255,0.15); }
.section-title {
    font-family: 'Sora', sans-serif;
    font-size: clamp(2rem, 3vw, 2.75rem);
    font-weight: 800; line-height: 1.2;
    color: var(--text); letter-spacing: -1px;
}
.section-title.inverted { color: #fff; }
.section-sub { color: var(--muted); font-size: 1.05rem; margin-top: 0.75rem; }

/* ── Hero ──────────────────────────────────── */
.hero-text { padding: 5rem 0 4rem 3rem; }
.hero-badge {
    display: inline-block;
    background: rgba(245,166,35,0.1); color: var(--accent);
    font-size: 0.8rem; font-weight: 600; letter-spacing: 0.5px;
    padding: 0.35rem 1rem; border-radius: 100px;
    border: 1px solid rgba(245,166,35,0.25); margin-bottom: 1.5rem;
}
.hero-headline {
    font-family: 'Sora', sans-serif;
    font-size: clamp(2.8rem, 4.5vw, 4.25rem);
    font-weight: 800; line-height: 1.1;
    letter-spacing: -2px; margin-bottom: 1.25rem;
}
.hero-accent {
    background: linear-gradient(90deg, var(--accent), var(--accent2));
    -webkit-background-clip: text; -webkit-text-fill-color: transparent;
}
.hero-sub {
    color: var(--muted); font-size: 1.1rem; line-height: 1.7;
    max-width: 420px; margin-bottom: 2rem;
}
.hero-buttons { display: flex; gap: 1rem; flex-wrap: wrap; margin-bottom: 1.5rem; }
.btn-primary {
    background: var(--accent); color: #0a0a0f;
    padding: 0.75rem 1.75rem; border-radius: 8px;
    font-weight: 700; font-size: 0.95rem;
    text-decoration: none; letter-spacing: -0.2px;
    transition: opacity 0.2s; display: inline-block;
}
.btn-primary:hover { opacity: 0.88; }
.btn-primary.large { padding: 0.9rem 2rem; font-size: 1rem; }
.btn-ghost {
    background: transparent; color: var(--text);
    padding: 0.75rem 1.75rem; border-radius: 8px;
    font-weight: 600; font-size: 0.95rem;
    text-decoration: none; border: 1px solid var(--border);
    transition: border-color 0.2s; display: inline-block;
}
.btn-ghost:hover { border-color: rgba(255,255,255,0.25); }
.btn-ghost.light { color: #fff; border-color: rgba(255,255,255,0.3); }
.btn-ghost.light:hover { border-color: #fff; }
.btn-ghost.large { padding: 0.9rem 2rem; font-size: 1rem; }
.hero-footnote { color: var(--muted); font-size: 0.8rem; letter-spacing: 0.2px; }

/* ── Mockup ────────────────────────────────── */
.hero-mockup {
    margin: 3rem 3rem 3rem 0;
    background: var(--bg3); border: 1px solid var(--border);
    border-radius: 14px; overflow: hidden;
    box-shadow: 0 30px 80px rgba(0,0,0,0.5);
}
.mockup-bar {
    background: var(--bg2); padding: 0.65rem 1rem;
    display: flex; align-items: center; gap: 0.5rem;
    border-bottom: 1px solid var(--border);
}
.dot { width: 10px; height: 10px; border-radius: 50%; display: inline-block; }
.dot.red    { background: #f87171; }
.dot.yellow { background: #fbbf24; }
.dot.green  { background: #4ade80; }
.mockup-url {
    flex: 1; text-align: center; font-size: 0.72rem;
    color: var(--muted); background: var(--bg3);
    padding: 0.2rem 0.75rem; border-radius: 4px;
    border: 1px solid var(--border);
}
.mockup-nav {
    display: flex; justify-content: space-between; align-items: center;
    padding: 0.75rem 1rem; border-bottom: 1px solid var(--border);
}
.mockup-logo { font-weight: 700; font-size: 0.85rem; color: var(--accent); }
.mockup-navlinks { font-size: 0.72rem; color: var(--muted); }
.mockup-hero-banner {
    background: linear-gradient(135deg, rgba(245,166,35,0.15), rgba(255,107,107,0.1));
    padding: 1.5rem 1rem; text-align: center;
    border-bottom: 1px solid var(--border);
}
.mockup-banner-text { font-family: 'Sora', sans-serif; font-weight: 700; font-size: 1rem; margin-bottom: 0.25rem; }
.mockup-banner-sub  { font-size: 0.72rem; color: var(--muted); margin-bottom: 0.75rem; }
.mockup-banner-btn  {
    display: inline-block; background: var(--accent); color: #0a0a0f;
    font-size: 0.72rem; font-weight: 700;
    padding: 0.3rem 0.9rem; border-radius: 4px;
}
.mockup-grid {
    display: grid; grid-template-columns: repeat(3, 1fr);
    gap: 0.65rem; padding: 0.75rem;
}
.mockup-card {
    background: var(--bg2); border: 1px solid var(--border);
    border-radius: 8px; overflow: hidden;
}
.mockup-img {
    height: 64px; background: rgba(255,255,255,0.05);
    border-bottom: 1px solid var(--border);
}
.mockup-img.accent { background: rgba(245,166,35,0.1); }
.mockup-img.soft   { background: rgba(255,107,107,0.08); }
.mockup-card-name  { font-size: 0.65rem; font-weight: 600; padding: 0.4rem 0.5rem 0.1rem; }
.mockup-card-price { font-size: 0.62rem; color: var(--accent); padding: 0 0.5rem 0.3rem; font-weight: 700; }
.mockup-card-btn   {
    font-size: 0.6rem; text-align: center;
    background: rgba(245,166,35,0.12); color: var(--accent);
    padding: 0.25rem; margin: 0.25rem 0.5rem 0.5rem;
    border-radius: 3px; font-weight: 600;
}

/* ── Stats bar ─────────────────────────────── */
.stat-card {
    text-align: center; padding: 1.75rem 1rem;
    border-left: 1px solid var(--border);
}
.stat-value {
    font-family: 'Sora', sans-serif;
    font-size: 2rem; font-weight: 800;
    color: var(--accent); letter-spacing: -1px;
}
.stat-label { color: var(--muted); font-size: 0.85rem; margin-top: 0.25rem; }

/* ── Feature cards ─────────────────────────── */
.feature-card {
    background: var(--bg2); border: 1px solid var(--border);
    border-radius: 12px; padding: 1.75rem 1.5rem;
    margin-bottom: 1rem; height: 100%;
    transition: border-color 0.2s, transform 0.2s;
}
.feature-card:hover { border-color: rgba(245,166,35,0.35); transform: translateY(-3px); }
.feature-icon  { font-size: 1.75rem; margin-bottom: 0.75rem; }
.feature-title { font-family: 'Sora', sans-serif; font-weight: 700; font-size: 1rem; margin-bottom: 0.4rem; }
.feature-desc  { color: var(--muted); font-size: 0.875rem; line-height: 1.6; }

/* ── Steps ─────────────────────────────────── */
.step-card {
    padding: 2rem 1.5rem; text-align: center;
    position: relative;
}
.step-number {
    font-family: 'Sora', sans-serif;
    font-size: 3.5rem; font-weight: 800;
    color: rgba(245,166,35,0.15); line-height: 1;
    margin-bottom: 0.75rem;
}
.step-title { font-family: 'Sora', sans-serif; font-weight: 700; font-size: 1.15rem; margin-bottom: 0.5rem; }
.step-desc  { color: var(--muted); font-size: 0.9rem; line-height: 1.6; }

/* ── Pricing cards ─────────────────────────── */
.popular-badge {
    background: var(--accent); color: #0a0a0f;
    text-align: center; font-size: 0.75rem; font-weight: 800;
    letter-spacing: 0.5px; padding: 0.35rem 1rem;
    border-radius: 8px 8px 0 0;
}
.popular-badge.invisible { opacity: 0; pointer-events: none; }
.pricing-card {
    background: var(--bg2); border: 1px solid var(--border);
    border-radius: 0 0 12px 12px; padding: 2rem 1.5rem 1.5rem;
    margin-bottom: 1rem;
}
.pricing-card.popular {
    background: var(--bg3);
    border-color: var(--accent); border-top: none;
}
.plan-name   { font-family: 'Sora', sans-serif; font-weight: 700; font-size: 1rem; color: var(--muted); margin-bottom: 0.5rem; text-transform: uppercase; letter-spacing: 1px; }
.plan-price  { font-family: 'Sora', sans-serif; font-size: 2.5rem; font-weight: 800; letter-spacing: -1.5px; margin-bottom: 0.25rem; }
.plan-period { font-size: 1rem; font-weight: 400; color: var(--muted); }
.plan-divider { border: none; border-top: 1px solid var(--border); margin: 1.25rem 0; }
.plan-feature { font-size: 0.875rem; padding: 0.35rem 0; color: var(--text); display: flex; gap: 0.5rem; }
.tick-yes { color: var(--green); font-weight: 700; }
.tick-no  { color: var(--muted); }
.plan-cta {
    display: block; text-align: center; margin-top: 1.5rem;
    background: var(--accent); color: #0a0a0f;
    padding: 0.75rem; border-radius: 8px;
    font-weight: 700; font-size: 0.9rem;
    text-decoration: none; transition: opacity 0.2s;
}
.plan-cta:hover { opacity: 0.88; }
.pricing-card.popular .plan-cta { background: var(--accent); }

/* ── Testimonials ──────────────────────────── */
.testimonial-card {
    background: var(--bg2); border: 1px solid var(--border);
    border-radius: 12px; padding: 1.75rem 1.5rem;
    margin-bottom: 1rem; position: relative;
}
.quote-mark { font-size: 4rem; line-height: 1; color: var(--accent); opacity: 0.4; font-family: Georgia, serif; }
.quote-text { font-size: 0.925rem; line-height: 1.7; color: var(--text); margin: 0.5rem 0 1.25rem; }
.quote-author { display: flex; align-items: center; gap: 0.75rem; }
.author-avatar {
    width: 36px; height: 36px; border-radius: 50%;
    background: rgba(245,166,35,0.2); color: var(--accent);
    display: flex; align-items: center; justify-content: center;
    font-weight: 700; font-size: 0.9rem; flex-shrink: 0;
}
.author-name  { font-weight: 600; font-size: 0.875rem; }
.author-store { color: var(--muted); font-size: 0.78rem; }

/* ── CTA Banner ────────────────────────────── */
.cta-banner {
    background: linear-gradient(135deg, rgba(245,166,35,0.1), rgba(255,107,107,0.06));
    border: 1px solid rgba(245,166,35,0.2);
    border-radius: 16px; margin: 1rem 3rem;
    padding: 4rem 2rem; text-align: center;
}
.cta-tag   { font-size: 0.78rem; font-weight: 700; letter-spacing: 1px; text-transform: uppercase; color: var(--accent); margin-bottom: 1rem; }
.cta-title { font-family: 'Sora', sans-serif; font-size: clamp(2rem, 3.5vw, 3rem); font-weight: 800; letter-spacing: -1.5px; margin-bottom: 0.75rem; }
.cta-sub   { color: var(--muted); font-size: 1.05rem; margin-bottom: 2rem; }
.cta-buttons { display: flex; gap: 1rem; justify-content: center; flex-wrap: wrap; }

/* ── Footer ────────────────────────────────── */
.footer-brand    { padding: 1rem 0; }
.footer-logo     { font-family: 'Sora', sans-serif; font-size: 1.15rem; font-weight: 700; color: var(--accent); margin-bottom: 0.6rem; }
.footer-tagline  { color: var(--muted); font-size: 0.85rem; line-height: 1.6; }
.footer-social   { display: flex; gap: 0.75rem; margin-top: 1rem; }
.footer-social span { width: 32px; height: 32px; border: 1px solid var(--border); border-radius: 6px; display: flex; align-items: center; justify-content: center; font-size: 0.75rem; color: var(--muted); cursor: pointer; }
.footer-col      { padding: 1rem 0; }
.footer-col-heading { font-weight: 700; font-size: 0.8rem; text-transform: uppercase; letter-spacing: 1px; color: var(--text); margin-bottom: 0.75rem; }
.footer-link     { display: block; color: var(--muted); text-decoration: none; font-size: 0.875rem; padding: 0.25rem 0; transition: color 0.2s; }
.footer-link:hover { color: var(--text); }
.footer-bottom   {
    border-top: 1px solid var(--border);
    padding: 1.25rem 3rem;
    display: flex; justify-content: space-between; flex-wrap: wrap; gap: 0.5rem;
    font-size: 0.78rem; color: var(--muted);
}
.footer-bottom a { color: var(--muted); text-decoration: none; }
.footer-bottom a:hover { color: var(--text); }
</style>
""", unsafe_allow_html=True)

# ── Sidebar chatbot ────────────────────────────────────────────────────────
render_chatbot()

# ── Page assembly ──────────────────────────────────────────────────────────
render_navbar()
render_hero()
render_stats()
render_features()
render_how_it_works()
render_pricing()
render_testimonials()
render_cta()
render_footer()
