%-------------------------------------------------------------------------------
  %
  %  Filename       : survey_dbscan.m
  %  Author         : Huang Leilei
  %  Created        : 2020-07-19
  %  Description    : survey_dbscan
  %
%-------------------------------------------------------------------------------

%*** INIT **********************************************************************
clc;
clear;
close all;
if ~exist('dump', 'dir')
    mkdir dump
end
tic;


%*** PARAMETER *****************************************************************
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;


%*** DERIVED PARAMETER *********************************************************


%*** MAIN BODY *****************************************************************
% open file
fpt = fopen('import/07-01-17-27-autoSave_11_Tatget_2020_07_01_17_27_33_CTLRR-300-V2_1_RawTarget.csv');

% open figure
figure(1);
set(gcf, 'position', [2, 42, 958, 953]);

% core loop
datFul = ones(128, 4);
idx = 1;
while ~feof(fpt)
    datStr = fgetl(fpt);
    %fprintf("%s", datFul);
    datTokens = regexp(datStr, '^\d+,([0-9.-]+),([0-9.-]+),([0-9.-]+),([0-9.-]+),.*',  'tokens');
    if ~isempty(datTokens)
        for i = 1:4
            datFul(idx, i) = str2double(datTokens{1}{i});
        end
        idx = idx + 1;
    end
    if strcmp(datStr(1:3), 'END')
        % filter
        datLst = datFul(1:idx-1, :);

        % convert
        datLst(:, IDX_ANG) = datLst(:, IDX_ANG) / 180 * pi;

        % cluster
        [idxGrp, idxKnl] = dbscan(datLst, 5, 1, 'distance', @cstCustom);
        %[idxGrp, idxKnl] = dbscanMine1(datLst, 5, 1, @cstCustom);

        % reset figure
        clf;

        % plot before cluster
        subplot(1,2,1);
        hold on;
        % +
        idxFlt = datLst(:,IDX_VEL) == 0;
        plot(datLst(idxFlt,IDX_RNG) .* sin(datLst(idxFlt,IDX_ANG)), datLst(idxFlt,IDX_RNG) .* cos(datLst(idxFlt,IDX_ANG)), 'og');
        % 0
        idxFlt = datLst(:,IDX_VEL) > 0;
        plot(datLst(idxFlt,IDX_RNG) .* sin(datLst(idxFlt,IDX_ANG)), datLst(idxFlt,IDX_RNG) .* cos(datLst(idxFlt,IDX_ANG)), 'ob');
        % -
        idxFlt = datLst(:,IDX_VEL) < 0;
        plot(datLst(idxFlt,IDX_RNG) .* sin(datLst(idxFlt,IDX_ANG)), datLst(idxFlt,IDX_RNG) .* cos(datLst(idxFlt,IDX_ANG)), 'or');
        % set figure
        title('before cluster (color is used to indicate speed)');
        axis equal;
        axis([-25, 25, 0, 100]);
        grid on;

        % plot after cluster
        subplot(1,2,2);
        hold on;
        % group
        for i = 1:max(idxGrp)
            idxFlt = idxGrp == i & idxKnl == 1;
            plot(datLst(idxFlt,IDX_RNG) .* sin(datLst(idxFlt,IDX_ANG)), datLst(idxFlt,IDX_RNG) .* cos(datLst(idxFlt,IDX_ANG)), 'o');
            idxFlt = idxGrp == i & idxKnl == 0;
            plot(datLst(idxFlt,IDX_RNG) .* sin(datLst(idxFlt,IDX_ANG)), datLst(idxFlt,IDX_RNG) .* cos(datLst(idxFlt,IDX_ANG)), 'x');
        end
        idxFlt = idxGrp == -1 & idxKnl == 0;
        plot(datLst(idxFlt,IDX_RNG) .* sin(datLst(idxFlt,IDX_ANG)), datLst(idxFlt,IDX_RNG) .* cos(datLst(idxFlt,IDX_ANG)), 's');
        % set figure
        title('after cluster (color is used to indicate group)');
        axis equal;
        axis([-25, 25, 0, 100]);
        grid on;

        % draw now
        drawnow;

        % reset idx
        idx = 1;
    end
end

% close file
fclose(fpt);


%*** POST **********************************************************************
toc;
