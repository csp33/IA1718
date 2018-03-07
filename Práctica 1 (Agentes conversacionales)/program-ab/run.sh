#/bin/bash
# Run bot
rm -rf bots/mybot/aimlif/*
echo "aimlif eliminado"
java -cp lib/Ab.jar Main bot=mybot action=chat trace=false morph=false
