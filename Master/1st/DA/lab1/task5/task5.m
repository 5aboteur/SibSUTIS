%init x and y
x = -5:0.01:5;
y = (5 + x).*(5 - x);

% draw graph
plot(x, y);
axis([-6 6 0 26]);
axis('square');
grid on;
hold on;

% mark points
plot(-5, 0, 'ok', 5, 0, 'ok');
legend('y = (5 + x)(5 - x)', 'Location', 'south', 'Orientation', 'vertical');
xlabel('X');
ylabel('Y');
grid on;
