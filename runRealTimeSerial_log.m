function runRealTimeSerial_log(portName, baudRate)
% runRealTimeSerial_log(port, baud): 实时绘图并把 (t,v) 保存到文件
% 例: runRealTimeSerial_log("COM7",9600)

%% ——— 打开串口 ———
s = serialport(portName, baudRate, Timeout=5);
flush(s);
disp("开始读取，按 × 或 Ctrl+C 结束并保存数据");

%% ——— 图窗 ———
fig = figure('Name','实时数据','NumberTitle','off',...
    'CloseRequestFcn',@onFigureClose);
hLine = animatedline('LineWidth',1.5,'MaximumNumPoints',5000);
ax = gca; grid on; xlabel('Time (s)'); ylabel('Value');
ax.XLim = [0 10]; ax.YLim = [-1 1];

%% ——— 数据容器 ———
tStart = tic; tAll = []; vAll = [];
lastT = NaN; lastV = NaN;

%% ——— 注册回调 ———
configureCallback(s,"terminator",@onSerialLine);

uiwait(fig);          % 阻塞，直到窗口关闭

%% ——— 保存数据 ———
if isempty(tAll)
    warning('无数据可保存'); return
end
tsFile = datestr(now,'yyyymmdd_HHMMSS');
matName = "serial_data_" + tsFile + ".mat";
csvName = "serial_data_" + tsFile + ".csv";
save(matName,'tAll','vAll');
writematrix([tAll(:) vAll(:)], csvName);
fprintf("📁 数据已保存到\n  %s\n  %s\n", matName, csvName);

%% ===== 内部函数 =====
    function onSerialLine(src,~)
        line = readline(src);
        nums = sscanf(line,'%f');
        if isempty(nums), return; end
        v = nums(1);
        t = toc(tStart);

        if t==lastT && v==lastV, return; end
        lastT = t; lastV = v;

        % 打印简洁行
        fprintf('%.6g\n', v);

        % 绘图
        addpoints(hLine,t,v);
        ax.XLim = [max(0,t-10) t+0.5];
        if v<ax.YLim(1) || v>ax.YLim(2)
            ax.YLim = [min(ax.YLim(1),v-1) max(ax.YLim(2),v+1)];
        end
        drawnow limitrate

        % 追加数据
        tAll(end+1) = t;
        vAll(end+1) = v;
    end

    function onFigureClose(~,~)
        % 关闭回调与串口，退出 uiwait
        configureCallback(s,"off");
        delete(s);
        uiresume(fig); delete(fig);
    end
end
