#!/bin/bash
# Start Deskflow server, optionally stopping Synergy first

DESKFLOW_BIN="/Users/gheath/Documents/deskflow/deskflow/build/bin/Deskflow.app/Contents/MacOS/deskflow-core"
PORT=24800

# Check if Synergy is running
SYNERGY_PID=$(lsof -ti :$PORT 2>/dev/null | head -1)

if [ -n "$SYNERGY_PID" ]; then
    echo "⚠️  Synergy is running on port $PORT (PID: $SYNERGY_PID)"
    echo ""
    echo "Options:"
    echo "  1. Stop Synergy and start Deskflow on port $PORT"
    echo "  2. Start Deskflow on a different port (24801)"
    echo "  3. Cancel"
    echo ""
    read -p "Choose option [1-3]: " choice

    case $choice in
        1)
            echo "Stopping Synergy..."
            kill $SYNERGY_PID
            sleep 2
            if kill -0 $SYNERGY_PID 2>/dev/null; then
                echo "⚠️  Synergy didn't stop, trying force kill..."
                kill -9 $SYNERGY_PID
            fi
            echo "✅ Synergy stopped"
            ;;
        2)
            PORT=24801
            echo "Using port $PORT instead"
            ;;
        3)
            echo "Cancelled"
            exit 0
            ;;
        *)
            echo "Invalid choice, cancelling"
            exit 1
            ;;
    esac
fi

echo ""
echo "🚀 Starting Deskflow server on port $PORT..."
echo "   Server IPs: 192.168.1.137 / 192.168.1.206"
echo "   Client should connect to: 192.168.1.137:$PORT"
echo ""

if [ "$PORT" != "24800" ]; then
    # Use custom port via config or environment
    echo "⚠️  Note: Using non-standard port $PORT"
    echo "   Client will need: --port $PORT"
fi

$DESKFLOW_BIN server --new-instance

