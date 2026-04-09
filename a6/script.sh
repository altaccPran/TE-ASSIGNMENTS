#!/usr/bin/env bash
# Deploy TE-ASSIGNMENTS/a6/StudentApp to Homebrew Tomcat on macOS.
# Usage: ./deploy-studentapp.sh
# Optional: STUDENTAPP_SRC=/path/to/StudentApp ./deploy-studentapp.sh

set -euo pipefail

# --- adjust if your project lives elsewhere ---
STUDENTAPP_SRC="${STUDENTAPP_SRC:-/Users/froncort.ai/school/TE-ASSIGNMENTS/a6/StudentApp}"

# Homebrew Tomcat 9: CATALINA_HOME is libexec (contains lib/, webapps/)
if [[ -z "${CATALINA_HOME:-}" ]]; then
  if command -v brew >/dev/null 2>&1; then
    CATALINA_HOME="$(brew --prefix tomcat@9)/libexec"
  else
    echo "Set CATALINA_HOME to your Tomcat root (folder with bin/, lib/, webapps/)."
    exit 1
  fi
fi

DEST="${CATALINA_HOME}/webapps/StudentApp"

echo "CATALINA_HOME=$CATALINA_HOME"
echo "Source:      $STUDENTAPP_SRC"
echo "Destination: $DEST"

if [[ ! -d "$STUDENTAPP_SRC" ]]; then
  echo "ERROR: Source folder not found: $STUDENTAPP_SRC"
  exit 1
fi
if [[ ! -d "${CATALINA_HOME}/webapps" ]]; then
  echo "ERROR: No webapps under CATALINA_HOME: ${CATALINA_HOME}/webapps"
  exit 1
fi

# Remove old deployment, then copy fresh tree (includes display_students.jsp + WEB-INF/lib/*.jar)
rm -rf "$DEST"
mkdir -p "$(dirname "$DEST")"
cp -R "$STUDENTAPP_SRC" "$DEST"

echo "Deployed OK."
echo "Open: http://localhost:8080/StudentApp/display_students.jsp"
echo ""
echo "Restart Tomcat if the app was already loaded (needed after JAR or JSP changes):"
echo "  brew services restart tomcat@9"