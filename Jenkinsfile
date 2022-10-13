pipeline {
    agent {
        label 'wotv:android'
    }


    stages ('build'){
                steps {
                script {
                    -- echo "......当前路径为：${env.WORKSPACE}......"
                    echo "......开始拉取最新 git 资源......"
                    echo "......完成拉取最新 git 资源......"
                }
            }
    }


      stage('dSYM Upload') {
        echo "......dSYM Upload......"

      }

}