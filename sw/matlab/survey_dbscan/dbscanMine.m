%%*** MAIN FUNCTION ************************************************************
function varargout = dbscanMine(varargin)
    gui_Singleton = 1;
    gui_State = struct('gui_Name',       mfilename, ...
        'gui_Singleton',  gui_Singleton, ...
        'gui_OpeningFcn', @dbscanMine_OpeningFcn, ...
        'gui_OutputFcn',  @dbscanMine_OutputFcn, ...
        'gui_LayoutFcn',  [] , ...
        'gui_Callback',   []);
    if nargin && ischar(varargin{1})
        gui_State.gui_Callback = str2func(varargin{1});
    end

    if nargout
        [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
    else
        gui_mainfcn(gui_State, varargin{:});
    end
% end of dbscanMine

function dbscanMine_OpeningFcn(hObject, eventdata, handles, varargin)
    handles.output = hObject;
    guidata(hObject, handles);
% end of dbscanMine_OpeningFcn

function varargout = dbscanMine_OutputFcn(hObject, eventdata, handles)
    varargout{1} = handles.output;
% end of dbscanMine_OutputFcn


%%*** SUB FUNCTION *************************************************************
function editCur_Callback(hObject, eventdata, handles)
    idxFra = get(handles.editCur, 'string');
    idxFra = str2double(idxFra);
    idxFra = min(handles.idxFraMax, max(handles.idxFraMin, idxFra));
    set(handles.editCur, 'string', num2str(idxFra));
    logAftGrp(handles)
    if get(handles.radiobuttonPlayBack, 'value')
        handles = drawAftGrp(handles, 0);
        handles = drawAftGrp(handles, 1);
    else
        drawBfrGrp(handles);
        handles = drawAftGrp(handles, 1);
    end
    drawnow;
    guidata(hObject, handles);
% end of editCur_Callback

function editCur_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of editCur_CreateFcn

function editFileName_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of editFileName_CreateFcn

function editOutput_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of editOutput_CreateFcn


function editBoundaryRng_Callback(hObject, eventdata, handles)
% end of editBoundaryRng_Callback

function editBoundaryRng_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of editBoundaryRng_CreateFcn

function editBoundaryVelUp_Callback(hObject, eventdata, handles)
% end of editBoundaryVelUp_Callback

function editBoundaryVelUp_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of editBoundaryVelUp_CreateFcn

function editBoundaryVelDn_Callback(hObject, eventdata, handles)
% end of editBoundaryVelDn_Callback

function editBoundaryVelDn_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of editBoundaryVelDn_CreateFcn

function editBoundaryAng_Callback(hObject, eventdata, handles)
% end of editBoundaryAng_Callback

function editBoundaryAng_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of editBoundaryAng_CreateFcn

function popupmenuAlgo_CreateFcn(hObject, eventdata, handles)
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
% end of popupmenu1_CreateFcn

function popupmenuAlgo_Callback(hObject, eventdata, handles)
    if get(handles.radiobuttonPlayBack, 'value')
        handles = drawAftGrp(handles, 0);
        handles = drawAftGrp(handles, 1);
    else
        drawBfrGrp(handles);
        handles = drawAftGrp(handles, 1);
    end
    logAftGrp(handles);
    drawnow;
    guidata(hObject, handles);
% end of popupmenu1_Callback

function pushbuttonload_Callback(hObject, eventdata, handles)
    % before everything
    set(handles.textBusy, 'string', 'busy');

    % open file
    if get(handles.radiobuttonPlayBack, 'value')
        [fileName, filePath] = uigetfile({'*.log'});
    else
        [fileName, filePath] = uigetfile({'*.csv'});
    end
    handles.filePath = filePath;
    if fileName == 0
        set(handles.editFileName, 'string', 'NO file is opened')
        return
    end
    set(handles.editFileName, 'string', fileName)
    fpt = fopen([filePath, fileName], 'r');

    % parameter
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;
    IDX_IDO = 5;
    IDX_KNL = 6;
    IDX_IDM = 7;
    NUM_DAT_MAX = 128;
    NUM_FRA_AVE = 1024;

    % variables
            cntPntFra = ones(             1          , 1      );
    handles.cntPntAll = ones(NUM_FRA_AVE, 1          , 1      );
    if get(handles.radiobuttonPlayBack, 'value')
            datPntFra = ones(             NUM_DAT_MAX, IDX_IDM);
    else
            datPntFra = ones(             NUM_DAT_MAX, IDX_SNR);
    end
    handles.datPntAll = ones(NUM_FRA_AVE, NUM_DAT_MAX, IDX_IDM);
    handles.idxFraMin = inf;
            idxFra    = [];
    handles.idxFraMax = -inf;

    % main loop
    while (1)
        cntPntFra = 0;
        while (1)
            % get one line
            datStr = fgetl(fpt);

            % terminate
            if datStr == -1
                break;
            end
            if isempty(datStr)
                break;
            end

            % grep frame index
            if get(handles.radiobuttonPlayBack, 'value')
                datGot = regexp(datStr, 'BGN OF FRAME ([0-9]+)', 'tokens');
            else
                datGot = regexp(datStr, 'FrameNb:([0-9]+)', 'tokens');
            end
            if ~isempty(datGot)
                idxFra = str2double(datGot{1}{1});
                set(handles.editCur, 'string', num2str(idxFra));
                drawnow;

                % min
                if (idxFra < handles.idxFraMin)
                    handles.idxFraMin = idxFra;
                end

                % max
                if (idxFra > handles.idxFraMax)
                    handles.idxFraMax = idxFra;
                end
            end

            % grep information
            if get(handles.radiobuttonPlayBack, 'value')
                datGot = regexp(datStr, '^\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)\s+([0-9.-]+)', 'tokens');
            else
                datGot = regexp(datStr, '^\d+,([0-9.-]+),([0-9.-]+),([0-9.-]+),([0-9.-]+)', 'tokens');
            end
            if ~isempty(datGot)
                cntPntFra = cntPntFra + 1;
                for i = 1:IDX_SNR
                    datPntFra(cntPntFra, i) = str2double(datGot{1}{i});
                end
                if get(handles.radiobuttonPlayBack, 'value')
                    datPntFra(cntPntFra, IDX_IDO) = str2double(datGot{1}{5});
                    datPntFra(cntPntFra, IDX_KNL) = 1;
                    datPntFra(cntPntFra, IDX_IDM) = str2double(datGot{1}{6});
                end
            end

            % grep end
            if strcmp(datStr(1:3), 'END')
                break;
            end
        end

        % terminate
        if datStr == -1
            break;
        end
        if isempty(datStr)
            break;
        end

        % filter
        if get(handles.radiobuttonPlayBack, 'value')
            cntPntFra = min(NUM_DAT_MAX, cntPntFra);
            datPntFra = datPntFra(1:cntPntFra, :);
        else
            cntPntFra = min(NUM_DAT_MAX, cntPntFra);
            if get(handles.radiobuttonTx1, 'value') && get(handles.radiobuttonTx2, 'value')
                datPntFra = datPntFra(1:cntPntFra, :);
            else
                for i = 1:cntPntFra
                    if datPntFra(i,1) == 1024 || i == cntPntFra
                        if get(handles.radiobuttonTx1, 'value')
                            datPntFra = datPntFra(1:i-1, :);
                            cntPntFra = i - 1;
                        else
                            datPntFra = datPntFra(i+1:cntPntFra, :);
                            cntPntFra = cntPntFra - i;
                        end
                        break;
                    end
                end
            end
        end

        % convert
        datPntFra(:, IDX_ANG) = datPntFra(:, IDX_ANG) / 180 * pi;

        % cluster
        if ~ get(handles.radiobuttonPlayBack, 'value')
            strAlgAll = cellstr(get(handles.popupmenuAlgo,'String'));
            idxAlg    =         get(handles.popupmenuAlgo,'Value' ) ;
            strAlg    = strAlgAll{idxAlg};
            if strcmp(strAlg, 'dbscan')
                datCstFra = pdist2(datPntFra, datPntFra, @cstCustom);
                %[idxGrpFra, idxKnlFra] = dbscan     (datCstFra, 10, 1, 'distance', 'precomputed');
                %[idxGrpFra, idxKnlFra] = dbscanMine1(datCstFra, 10, 1);
                [idxGrpFra, idxKnlFra] = dbscanMine2(datCstFra, 10, 1);
            end
            if strcmp(strAlg, 'boxCut')
                idxGrpFra = boxCut(datPntFra);
                idxKnlFra = ones(cntPntFra, 1);
            end
            %idxGrpFra = ones(cntPntFra, 1);
            %idxKnlFra = ones(cntPntFra, 1);
        end

        % save
            handles.cntPntAll(idxFra + 1                ) = cntPntFra;
        if get(handles.radiobuttonPlayBack, 'value')
            handles.datPntAll(idxFra + 1, 1:cntPntFra, :) = datPntFra;
        else
            handles.datPntAll(idxFra + 1, 1:cntPntFra, :) = [datPntFra, idxGrpFra, idxKnlFra, idxGrpFra];
        end
    end

    % after everthing
    set(handles.textBusy, 'string', '');

    % plot
    if get(handles.radiobuttonPlayBack, 'value')
        handles = drawAftGrp(handles, 0);
        handles = drawAftGrp(handles, 1);
    else
        drawBfrGrp(handles);
        handles = drawAftGrp(handles, 1);
    end

    % log
    set(handles.textMin, 'string', num2str(handles.idxFraMin));
    set(handles.editCur, 'string', num2str(handles.idxFraMin));
    set(handles.textMax, 'string', num2str(handles.idxFraMax));
    logAftGrp(handles);

    % save
    guidata(hObject, handles);
% end of pushbuttonload_Callback

function pushbuttonDump_Callback(hObject, eventdata, handles)
    % before everything
    set(handles.textBusy, 'string', 'busy');

    % open file
    fileName = get(handles.editFileName, 'string');
    strAlgAll = cellstr(get(handles.popupmenuAlgo,'String'));
    idxAlg    =         get(handles.popupmenuAlgo,'Value' ) ;
    strAlg    = strAlgAll{idxAlg};
    fpt = fopen([handles.filePath, fileName, '.', strAlg, '.group.log'], 'w');
    fprintf(fpt, '    %-7s %-7s %-7s %-7s %-5s %-5s %s\n', 'RNG', 'VEL', 'ANG', 'SNR', 'ID(O)', 'ID(M)', 'CHANGED');

    % parameter
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;
    IDX_IDO = 5;
    IDX_KNL = 6;
    IDX_IDM = 7;

    % main loop
    numDifM2OAll = 0;
    numDifO2MAll = 0;
    for idxFra = handles.idxFraMin:handles.idxFraMax
        % log
        set(handles.editCur, 'string', num2str(idxFra));
        drawnow;

        % get data
        cntPnt         = handles.cntPntAll(idxFra + 1);
        datPnt         = ones(cntPnt, IDX_SNR);
        idxGrpOri      = ones(cntPnt, 1);
        idxGrpMod      = ones(cntPnt, 1);
           datPnt(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt, 1:IDX_SNR);
        idxGrpMod(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt,   IDX_IDM);

        % redo cluster
        %idxGrpOri(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt,   IDX_IDO);
        strAlgAll = cellstr(get(handles.popupmenuAlgo,'String'));
        idxAlg    =         get(handles.popupmenuAlgo,'Value' ) ;
        strAlg    = strAlgAll{idxAlg};
        if strcmp(strAlg, 'dbscan')
            datCst = pdist2(datPnt, datPnt, @cstCustom);
            [idxGrpOri, idxKnl] = dbscanMine2(datCst, 10, 1);
        end
        if strcmp(strAlg, 'boxCut')
            idxGrpOri = boxCut(datPnt);
            idxKnl    = ones(cntPnt, 1);
        end
        handles.datPntAll(idxFra + 1, 1:cntPnt, IDX_IDO) = idxGrpOri;

        % convert
        datPnt(:, IDX_ANG) = datPnt(:, IDX_ANG) / pi * 180;

        % dump
        fprintf(fpt, 'BGN OF FRAME %d\n', idxFra);
        fprintf(fpt, '    %-7.2f %-7.2f %-7.2f %-7.2f %02d    %02d    %d\n', [datPnt, idxGrpOri, idxGrpMod, idxGrpOri ~= idxGrpMod]');
        fprintf(fpt, 'END OF FRAME %d', idxFra);
        if sum(idxGrpOri ~= idxGrpMod)
            numDifM2O = calcDif(idxGrpOri, idxGrpMod, datPnt, handles); numDifM2OAll = numDifM2OAll + numDifM2O;
            numDifO2M = calcDif(idxGrpMod, idxGrpOri, datPnt, handles); numDifO2MAll = numDifO2MAll + numDifO2M;
            fprintf(fpt, ' MISMATCH: %d(M2O), %d(O2M)\n', numDifM2O, numDifO2M);
        else
            fprintf(fpt, '\n');
        end
    end
    fprintf(fpt, 'TOTAL MISMATCH: %d(M2O), %d(O2M)\n', numDifM2OAll, numDifO2MAll);

    % after everything
    set(handles.textBusy, 'string', '');

    % save
    guidata(hObject, handles);
% end of pushbuttonDump_Callback

function pushbuttonUp_Callback(hObject, eventdata, handles)
    idxFra = get(handles.editCur, 'string');
    idxFra = str2double(idxFra);
    idxFra = min(handles.idxFraMax, max(handles.idxFraMin, idxFra - 1));
    set(handles.editCur, 'string', num2str(idxFra));
    if get(handles.radiobuttonPlayBack, 'value')
        handles = drawAftGrp(handles, 0);
        handles = drawAftGrp(handles, 1);
    else
        drawBfrGrp(handles);
        handles = drawAftGrp(handles, 1);
    end
    logAftGrp(handles);
    drawnow;
    guidata(hObject, handles);
% end of pushbuttonUp_Callback

function pushbuttonDown_Callback(hObject, eventdata, handles)
    idxFra = get(handles.editCur, 'string');
    idxFra = str2double(idxFra);
    idxFra = min(handles.idxFraMax, max(handles.idxFraMin, idxFra + 1));
    set(handles.editCur, 'string', num2str(idxFra));
    if get(handles.radiobuttonPlayBack, 'value')
        handles = drawAftGrp(handles, 0);
        handles = drawAftGrp(handles, 1);
    else
        drawBfrGrp(handles);
        handles = drawAftGrp(handles, 1);
    end
    logAftGrp(handles);
    drawnow;
    guidata(hObject, handles);
% end of pushbuttonDown_Callback

function tableGroup_CellSelectionCallback(hObject, eventdata, handles)
    if (~isempty(eventdata.Indices))
        idxFlt = eventdata.Indices(1);
        idxFra = get(handles.editCur, 'string');
        idxFra = str2double(idxFra);
        cntPnt = handles.cntPntAll(idxFra + 1);
        if 1 <= idxFlt && idxFlt <= cntPnt
            if get(handles.radiobuttonPlayBack, 'value')
                handles = drawAftGrp(handles, 0);
                handles = drawAftGrp(handles, 1);
            else
                drawBfrGrp(handles);
                handles = drawAftGrp(handles, 1);
            end
            drawExtra(handles, idxFlt);
            drawnow;
        end
    end
    guidata(hObject, handles);
% end of tableGroup_CellSelectionCallback

function tableGroup_CellEditCallback(hObject, eventdata, handles)
    IDX_IDM = 7;
    idxFlt = eventdata.Indices(1);
    idxFra = get(handles.editCur, 'string');
    idxFra = str2double(idxFra);
    cntPnt = handles.cntPntAll(idxFra + 1);
    if 1 <= idxFlt && idxFlt <= cntPnt
        handles.datPntAll(idxFra + 1, idxFlt, IDX_IDM) = eventdata.NewData;
        if get(handles.radiobuttonPlayBack, 'value')
            handles = drawAftGrp(handles, 0);
            handles = drawAftGrp(handles, 1);
        else
            drawBfrGrp(handles);
            handles = drawAftGrp(handles, 1);
        end
        drawExtra(handles, idxFlt);
        drawnow;
    end
    guidata(hObject, handles);
% end of tableGroup_CellEditCallback

function radiobuttonTx1_Callback(hObject, eventdata, handles)
% end of radiobuttonTx1_Callback

function radiobuttonTx2_Callback(hObject, eventdata, handles)
% end of radiobuttonTx2_Callback

function radiobuttonFilter_Callback(hObject, eventdata, handles)
% end of radiobuttonFilter_Callback

function textMin_CreateFcn(hObject, eventdata, handles)
% end of textMin_CreateFcn

function textMin_DeleteFcn(hObject, eventdata, handles)
% end of textMin_DeleteFcn

function textMax_CreateFcn(hObject, eventdata, handles)
% end of textMax_CreateFcn

function textMax_DeleteFcn(hObject, eventdata, handles)
% end of textMax_DeleteFcn

function radiobuttonPlayBack_Callback(hObject, eventdata, handles)
    if get(handles.radiobuttonPlayBack, 'value')
        handles = drawAftGrp(handles, 0);
        handles = drawAftGrp(handles, 1);
    else
        drawBfrGrp(handles);
        handles = drawAftGrp(handles, 1);
    end
    drawnow;
    guidata(hObject, handles);
% end of radiobuttonPlayBack_Callback

function togglebuttonRun_Callback(hObject, eventdata, handles)
    while 1
        idxFra = get(handles.editCur, 'string');
        idxFra = str2double(idxFra);
        idxFra = min(handles.idxFraMax, max(handles.idxFraMin, idxFra + 1));
        set(handles.editCur, 'string', num2str(idxFra));
        if get(handles.radiobuttonPlayBack, 'value')
            handles = drawAftGrp(handles, 0);
            handles = drawAftGrp(handles, 1);
        else
            drawBfrGrp(handles);
            handles = drawAftGrp(handles, 1);
        end
        logAftGrp(handles);
        drawnow;
        if idxFra == handles.idxFraMax || get(handles.togglebuttonRun, 'value') == 0 || ~isempty(get(handles.textBusy, 'string'))
            set(handles.togglebuttonRun, 'value', 0);
            break;
        end
    end
    guidata(hObject, handles);
% end of togglebuttonRun_Callback


%%*** MY FUNCTION **************************************************************
function logAftGrp(handles)
    try
    % parameter
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;
    IDX_IDO = 5;
    IDX_KNL = 6;
    IDX_IDM = 7;

    % variable
    idxFra         = get(handles.editCur, 'string');
    idxFra         = str2double(idxFra);
    cntPnt         = handles.cntPntAll(idxFra + 1);
    datPnt         = ones(cntPnt, IDX_SNR);
    idxGrpOri      = ones(cntPnt, 1);
    idxGrpMod      = ones(cntPnt, 1);
    datPnt   (:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt, 1:IDX_SNR);
    idxGrpOri(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt, IDX_IDO);
    idxGrpMod(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt, IDX_IDM);

    % convert
    datPnt(:, IDX_ANG) = datPnt(:, IDX_ANG) / pi * 180;

    % log
    set(handles.tableGroup, 'data', [datPnt, idxGrpOri, idxGrpMod]);
    end
% end of logAftGrp

function drawBfrGrp(handles)
    try
    % parameter
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;
    IDX_IDO = 5;
    IDX_KNL = 6;
    IDX_IDM = 7;

    % variables
    idxFra      = get(handles.editCur, 'string');
    idxFra      = str2double(idxFra);
    cntPnt      = handles.cntPntAll(idxFra + 1);
    datPnt      = ones(cntPnt, IDX_SNR);
    datPnt(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt, 1:IDX_SNR);

    % plot
    axes(handles.axesInput);
    cla;
    hold on;
    % +
    idxFlt = datPnt(:,IDX_VEL) == 0;
    plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'og');
    % 0
    idxFlt = datPnt(:,IDX_VEL) > 0;
    plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'ob');
    % -
    idxFlt = datPnt(:,IDX_VEL) < 0;
    plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'or');
    % set figure
    %title('before cluster (color is used to indicate speed)');
    title(['before cluster (', num2str(idxFra), ')']);
    %axis equal;
    axis([-20, 20, 0, 100]);
    grid on;
    end
% end of drawBfrGrp

function [handles] = drawAftGrp(handles, flgMod)
    try
    % parameter
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;
    IDX_IDO = 5;
    IDX_KNL = 6;
    IDX_IDM = 7;

    % variables
       idxFra      = get(handles.editCur, 'string');
       idxFra      = str2double(idxFra);
       cntPnt      = handles.cntPntAll(idxFra + 1);
       datPnt      = ones(cntPnt, IDX_SNR);
       idxGrp      = ones(cntPnt, 1);
    idxGrpOri      = ones(cntPnt, 1);
    idxGrpMod      = ones(cntPnt, 1);
       idxKnl      = ones(cntPnt, 1);
       datPnt(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt, 1:IDX_SNR);
       idxKnl(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt,   IDX_KNL);
    idxGrpMod(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt,   IDX_IDM);

    % redo cluster
    %idxGrpOri(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt,   IDX_IDO);
    strAlgAll = cellstr(get(handles.popupmenuAlgo,'String'));
    idxAlg    =         get(handles.popupmenuAlgo,'Value' ) ;
    strAlg    = strAlgAll{idxAlg};
    if strcmp(strAlg, 'dbscan')
        datCst = pdist2(datPnt, datPnt, @cstCustom);
        [idxGrpOri, idxKnl] = dbscanMine2(datCst, 10, 1);
    end
    if strcmp(strAlg, 'boxCut')
        idxGrpOri = boxCut(datPnt);
        idxKnl    = ones(cntPnt, 1);
    end
    handles.datPntAll(idxFra + 1, 1:cntPnt, IDX_IDO) = idxGrpOri;

    % redirect
    if flgMod
        idxGrp = idxGrpMod;
    else
        idxGrp = idxGrpOri;
    end

    if flgMod
        if sum(idxGrpOri ~= idxGrpMod)
            set(handles.textBusy, 'string', ['dif: ', num2str(calcDif(idxGrpOri, idxGrp, datPnt, handles)), ',', num2str(calcDif(idxGrp, idxGrpOri, datPnt, handles))]);
        else
            set(handles.textBusy, 'string', '');
        end
    end

    % plot
    if flgMod
        axes(handles.axesOutput);
    else
        axes(handles.axesInput);
    end
    cla;
    hold on;
    for i = 1:max(idxGrp)
        datPntPosX = datPnt(:,IDX_RNG) .* sin(datPnt(:,IDX_ANG));
        datPntPosY = datPnt(:,IDX_RNG) .* cos(datPnt(:,IDX_ANG));
        idxFlt = idxGrp == i & -20 <= datPntPosX & datPntPosX <= 20 & 0 <= datPntPosY & datPntPosY <= 100;
        plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'o');
        idxFlt = find(idxFlt, 1);
        text(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)) + 1, datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), num2str(idxGrp(idxFlt)));
    end
    idxFlt = idxGrp == -1 & idxKnl == 0;
    plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 's');
    idxFlt = find(idxFlt, 1);
    text(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)) + 1, datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), num2str(idxGrp(idxFlt)));
    % set figure
    title(['after cluster (', num2str(idxFra), ')']);
    %axis equal;
    axis([-20, 20, 0, 100]);
    grid on;
    end
% end of drawAftGrp

function drawExtra(handles, idxFlt)
    try
    % parameter
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;
    IDX_IDO = 5;
    IDX_KNL = 6;
    IDX_IDM = 7;

    % variables
    idxFra      = get(handles.editCur, 'string');
    idxFra      = str2double(idxFra);
    cntPnt      = handles.cntPntAll(idxFra + 1);
    datPnt      = ones(cntPnt, IDX_SNR);
    datPnt(:,:) = handles.datPntAll(idxFra + 1, 1:cntPnt, 1:IDX_SNR);

    % plot
    axes(handles.axesInput);
    hold on;
    plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'xk', 'markersiz', 10);
    axes(handles.axesOutput);
    hold on;
    plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'xk', 'markersiz', 10);
    end
% end of drawExtra

function [numDif] = calcDif(datOri, datMod, datPnt, handles)
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;
    numDif = 0;
    for idxMod = min(datMod):max(datMod)
        % get current group
        idxGrp = datMod == idxMod;

        % filter
        if get(handles.radiobuttonFilter, 'value')
            idxGrp = idxGrp & datPnt(:,IDX_RNG) <= str2double(get(handles.editBoundaryRng  ,'string'))    ...
                            & datPnt(:,IDX_VEL) >= str2double(get(handles.editBoundaryVelDn,'string'))    ...
                            & datPnt(:,IDX_VEL) <= str2double(get(handles.editBoundaryVelUp,'string'))    ...
                            & datPnt(:,IDX_ANG) <= str2double(get(handles.editBoundaryAng  ,'string')) / 180 * pi;
        end

        % find corresponding groups
        datGrpOri = datOri(idxGrp);

        % find the most largest hitting group in ori
        cntHitBst = 0;
        for idxOri = min(datGrpOri):max(datGrpOri)
            cntHitCur = sum(datGrpOri == idxOri);
            if cntHitCur > cntHitBst
                cntHitBst = cntHitCur;
            end
        end

        % accumulate
        numDif = numDif + sum(idxGrp) - cntHitBst;
    end
% end of calcDif
