#!/usr/bin/env bash
# Deploy StudentApp to manual Tomcat (/opt/tomcat) on Ubuntu

set -euo pipefail

# --- adjust this if needed ---
STUDENTAPP_SRC="${STUDENTAPP_SRC:-$HOME/31435_pran/TE-ASSIGNMENTS/a6/StudentApp}"

CATALINA_HOME="/opt/tomcat"
DEST="${CATALINA_HOME}/webapps/StudentApp"

echo "CATALINA_HOME=$CATALINA_HOME"
echo "Source:      $STUDENTAPP_SRC"
echo "Destination: $DEST"

# --- checks ---
if [[ ! -d "$STUDENTAPP_SRC" ]]; then
  echo "ERROR: Source folder not found: $STUDENTAPP_SRC"
  exit 1
fi

if [[ ! -d "${CATALINA_HOME}/webapps" ]]; then
  echo "ERROR: Tomcat webapps folder not found: ${CATALINA_HOME}/webapps"
  exit 1
fi

# --- deploy ---
echo "Removing old deployment..."
sudo rm -rf "$DEST"

echo "Copying new files..."
sudo cp -R "$STUDENTAPP_SRC" "$DEST"

# --- fix permissions (important on Ubuntu) ---
sudo chown -R $USER:$USER "$DEST"

echo "Deployment complete."
