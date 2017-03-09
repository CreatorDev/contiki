#!/usr/bin/env groovy

/**
 * Build Contiki Applications
 *
 */
def examples = [
    'hello-world', 
    'ping-ipv6',
    'trickle-library',
    'servreg-hack',
    'http-socket',
    'udp-ipv6',
    'er-rest-example',
    'webserver-ipv6',
]

def mikroe_apps = [
    'sample-app',
    'test-button',
    'test-i2c',
    'test-led',
    'test-lmciot',
    'test-motion',
    'test-proximity',
    'test-relay',
    'test-rtc',
    'test-thermo3',
]

properties([
    buildDiscarder(logRotator(numToKeepStr: '30')),
    parameters([
        stringParam(defaultValue: '26', description: 'Set 6LoWPAN Channel', name: 'CHANNEL'),
        stringParam(defaultValue: '0xabcd', description: 'Set 6LoWPAN PAN ID', name: 'PAN_ID'),
        stringParam(defaultValue: '1', description: 'Set Serial port to use for console', name: 'PADS'),
    ])
])

node('docker && imgtec') {
    def docker_image
    docker_image = docker.image "imgtec/creator-builder:latest"
    docker_image.inside {
        stage('Prepare') {
            checkout([$class: 'GitSCM',
                userRemoteConfigs: scm.userRemoteConfigs,
                branches: scm.branches,
                doGenerateSubmoduleConfigurations: scm.doGenerateSubmoduleConfigurations,
                submoduleCfg: scm.submoduleCfg,
                browser: scm.browser,
                gitTool: scm.gitTool,
                extensions: scm.extensions + [
                    [$class: 'CleanCheckout'],
                    [$class: 'PruneStaleBranch'],
                    [$class: 'SubmoduleOption']
                ],
            ])
        }
        stage('Build') {
            for (item in examples) {
                try {
                    sh "cd examples/${item} && make -j8 TARGET=mikro-e CHANNEL=${params.CHANNEL} \
                    PAN_ID=${params.PAN_ID} USE_SERIAL_PADS=${params.PADS}"
                    sh "cd examples/${item} && xc32-bin2hex *.mikro-e && mv *.hex ${WORKSPACE}/"
                } catch(exc) {
                    echo "Caught: ${exc}"
                    currentBuild.result = 'FAILURE'
                }
            }
            for (item in mikroe_apps) {
                try {
                    sh "cd platform/mikro-e/apps/${item} && make -j8 TARGET=mikro-e CHANNEL=${params.CHANNEL} \
                    PAN_ID=${params.PAN_ID} USE_SERIAL_PADS=${params.PADS}"
                    sh "cd platform/mikro-e/apps/${item} && xc32-bin2hex *.mikro-e && mv *.hex ${WORKSPACE}/"
                } catch(exc) {
                    echo "Caught: ${exc}"
                    currentBuild.result = 'FAILURE'
                }
            }
        }
        stage('Upload') {
            archiveArtifacts '*.hex'
            deleteDir()  // clean up the workspace to save space
        }
    }
}
