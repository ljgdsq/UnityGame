
pipeline {
    agent {
        label 'wotv:android'
    }
    
    stages {
        stage('Remove Last Build') {
            steps {
                 echo "......开始拉取最新 git 资源......"
            }
        }
        stage ('Validate') {
            steps {
                script {
          echo "......开始拉取最新 git 资源 Validate......"
                }
            }
        }

    }
}

