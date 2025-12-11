# Checking Deskflow Server on macOS (Machine One)

## 1. Check if server is running:
```bash
ps aux | grep deskflow
# or
pgrep -fl deskflow
```

## 2. Check if server is listening on port 24800:
```bash
lsof -i :24800
# or
netstat -an | grep 24800
```

Should show something like:
- `*:24800` or `0.0.0.0:24800` (listening on all interfaces) ✅
- `127.0.0.1:24800` (only localhost) ❌ - won't accept network connections

## 3. Check macOS Firewall:
System Settings → Network → Firewall → Options
- Make sure port 24800 is allowed, or temporarily disable firewall for testing

## 4. Server config location on macOS:
`~/Library/Deskflow/Deskflow.conf`

Should have:
```
[core]
coreMode=2
port=24800

[security]
tlsEnabled=false
```

## 5. Start server if not running:
```bash
deskflow-core server
# or use the GUI app
```
