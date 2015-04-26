exports.drawPoint = function(ctx, point) {
  ctx.save();
  ctx.fillStyle = 'red';
  ctx.beginPath();
  ctx.arc(point.x, point.y, 3, 0, Math.PI * 2, false);
  ctx.closePath();
  ctx.fill();
  return ctx.restore();
};

exports.drawTriangle = function(ctx, triangle) {
  ctx.save();
  ctx.fillStyle = typeof fillColor !== 'undefined' ? fillColor : 'rgba(0, 0, 0, 0.2)';
  ctx.strokeStyle = typeof strokeColor !== 'undefined' ? strokeColor : 'rgba(0, 0, 0, 1.0)';
  ctx.beginPath();
  ctx.moveTo(triangle.p1.x, triangle.p1.y);
  ctx.lineTo(triangle.p2.x, triangle.p2.y);
  ctx.lineTo(triangle.p3.x, triangle.p3.y);
  ctx.closePath();
  ctx.fill();
  ctx.stroke();
  return ctx.restore();
};
