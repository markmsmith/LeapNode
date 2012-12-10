{
  "targets": [
    {
      "target_name": "leap",
      "sources": [ "leap.cc", "NodeController.cc", "NodeListener.cc", "LeapConverter.cc" ],
      "include_dirs": ["./include"],
      "libraries": ["../libLeap.dylib"],
      'conditions': [
          [ 'OS=="mac"', {
                'postbuilds': [
                    {
                      'postbuild_name': 'Fix Library Link',
                      'action': [
                        'install_name_tool',
                        '-change',
                        '@loader_path/libLeap.dylib',
                        './libLeap.dylib',
                        '${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}'
                      ],
                    }
                ]
            }]
        ]
    }

  ]
}
