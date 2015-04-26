exports.drawPoint = (ctx, point) ->
    ctx.save()
    ctx.fillStyle = 'red'
    ctx.beginPath()
    ctx.arc(point.x, point.y, 3, 0, Math.PI * 2, false)
    ctx.closePath()
    ctx.fill()
    ctx.restore()

exports.drawTriangle = (ctx, triangle) ->
    ctx.save()
    ctx.fillStyle = if typeof(fillColor) isnt 'undefined' then fillColor else 'rgba(0, 0, 0, 0.2)'
    ctx.strokeStyle = if typeof(strokeColor) isnt 'undefined' then strokeColor else 'rgba(0, 0, 0, 1.0)'
    ctx.beginPath()
    ctx.moveTo(triangle.p1.x, triangle.p1.y)
    ctx.lineTo(triangle.p2.x, triangle.p2.y)
    ctx.lineTo(triangle.p3.x, triangle.p3.y)
    ctx.closePath()
    ctx.fill()
    ctx.stroke()
    ctx.restore()

exports.drawCircle = (ctx, circle) ->
    ctx.save()
    ctx.strokeStyle = if typeof(strokeColor) isnt 'undefined' then strokeColor else 'rgba(0, 0, 0, 1.0)'
    ctx.beginPath()
    ctx.arc(circle.center.x, circle.center.y, circle.radius, 0, Math.PI * 2, false)
    ctx.closePath()
    ctx.stroke()
    ctx.restore()
