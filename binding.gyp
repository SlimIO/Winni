{
    "targets": [
        {
            "target_name": "winni",
            "sources": [
                "winni.cpp",
                "src/NetworkAdapters.cpp"
            ],
            "libraries": [
                "Ws2_32.lib"
            ],
            "include_dirs": [
                "include",
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "ExceptionHandling": 1
                },
            }
        }
    ]
}
