DEFINES += "BUILD_DATE=\"\\\"`date --rfc-3339=seconds`\\\"\""
DEFINES += "BUILD_AUTHOR=\"\\\"`cd $$IN_PWD; git config --get user.name`\\\"\""
DEFINES += "COMMIT=\"\\\"`cd $$IN_PWD; git log -n 1 --pretty=format:'%H by %an'`\\\"\""

CONFIG(debug, debug|release) {
    message("debug mode")
}
else {
    message("release mode")
    DEFINES += NDEBUG
}
