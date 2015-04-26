gulp       = require 'gulp'
coffee     = require 'gulp-coffee'

files = './src/*.coffee'

gulp.task 'js', ->
    gulp.src files
        .pipe coffee
            bare: true
        .pipe gulp.dest './dist'

gulp.task 'watch', ['build'], ->
    gulp.watch files, ['js']

gulp.task 'build', ['js']
gulp.task 'default', ['build']
