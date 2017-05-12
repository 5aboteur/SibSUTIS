% init x
x = -5:1:15;

% init ys
y1 = x.^2 - 5.*x + 3;
y2 = 7.*x - 4;

% solve the equations
sx1 = fsolve('x.^2 - 5.*x + 3 - 7.*x + 4', 1, optimset('Display', 'off'))
sy1 = sx1^2 - 5*sx1 + 3
sx2 = fsolve('x.^2 - 5.*x + 3 - 7.*x + 4', 12, optimset('Display', 'off'))
sy2 = sx2^2 - 5*sx2 + 3

% create graph with stuff
plot(x, y1, x, y2, sx1, sy1, 'ok', sx2, sy2, 'ok');
legend('y = x^2 - 5x + 3','y = 7x - 4', 'Location', 'northwest', 'Orientation', 'vertical');
xlabel('X');
ylabel('Y');
grid;