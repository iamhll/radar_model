function varargout = dbscanMine(varargin)
% DBSCANMINE MATLAB code for dbscanMine.fig
%      DBSCANMINE, by itself, creates a new DBSCANMINE or raises the existing
%      singleton*.
%
%      H = DBSCANMINE returns the handle to a new DBSCANMINE or the handle to
%      the existing singleton*.
%
%      DBSCANMINE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in DBSCANMINE.M with the given input arguments.
%
%      DBSCANMINE('Property','Value',...) creates a new DBSCANMINE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before dbscanMine_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to dbscanMine_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help dbscanMine

% Last Modified by GUIDE v2.5 28-Sep-2020 21:38:26

% Begin initialization code - DO NOT EDIT
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
% End initialization code - DO NOT EDIT


% --- Executes just before dbscanMine is made visible.
function dbscanMine_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to dbscanMine (see VARARGIN)

% Choose default command line output for dbscanMine
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes dbscanMine wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = dbscanMine_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes during object creation, after setting all properties.
function editFileName_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editFileName (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbuttonload.
function pushbuttonload_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonload (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% before everything
set(handles.textBusy, 'string', 'busy');

% open file
[fileName, filePath] = uigetfile({'*.csv'});
handles.filePath = filePath;
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
        datPntFra = ones(             NUM_DAT_MAX, IDX_SNR);
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
        datGot = regexp(datStr, 'FrameNb:([0-9]+)', 'tokens');
        if ~isempty(datGot)
            idxFra = str2double(datGot{1}{1}) + 1;
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
        datGot = regexp(datStr, '^\d+,([0-9.-]+),([0-9.-]+),([0-9.-]+),([0-9.-]+)', 'tokens');
        if ~isempty(datGot)
            cntPntFra = cntPntFra + 1;
            for i = 1:4
                datPntFra(cntPntFra, i) = str2double(datGot{1}{i});
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
    %datPntLst = datPntFra(1:cntPntFra, :);
    cntPntFra = min(NUM_DAT_MAX, cntPntFra);
    if get(handles.radiobuttonTx1, 'value') && get(handles.radiobuttonTx2, 'value')
        datPntFra = datPntFra(1:cntPntFra, :);
    else
        for i = 1:cntPntFra
            if datPntFra(i,1) == 1024
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

    % convert
    datPntFra(:, IDX_ANG) = datPntFra(:, IDX_ANG) / 180 * pi;

    % cluster
    datCstFra = pdist2(datPntFra, datPntFra, @cstCustom);
    %[idxGrpFra, idxKnlFra] = dbscan     (datCstFra, 10, 1, 'distance', 'precomputed');
    %[idxGrpFra, idxKnlFra] = dbscanMine1(datCstFra, 10, 1);
    [idxGrpFra, idxKnlFra] = dbscanMine2(datCstFra, 10, 1);

    % save
    handles.cntPntAll(idxFra                ) = cntPntFra;
    handles.datPntAll(idxFra, 1:cntPntFra, :) = [datPntFra, idxGrpFra, idxKnlFra, idxGrpFra];
end

% log
set(handles.textMin, 'string', num2str(handles.idxFraMin));
set(handles.editCur, 'string', num2str(handles.idxFraMin));
set(handles.textMax, 'string', num2str(handles.idxFraMax));
logAftGrp(handles);

% plot
drawBfrGrp(handles);
drawAftGrp(handles);

% save
guidata(hObject, handles);

% after everthing
set(handles.textBusy, 'string', '');


% --- Executes on button press in pushbuttonDown.
function pushbuttonDown_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonDown (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% get data
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
idxFra = min(handles.idxFraMax, max(handles.idxFraMin, idxFra + 1));
set(handles.editCur, 'string', num2str(idxFra));
logAftGrp(handles);
drawBfrGrp(handles);
drawAftGrp(handles);
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function editOutput_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editOutput (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes when selected cell(s) is changed in tableGroup.
function tableGroup_CellSelectionCallback(hObject, eventdata, handles)
% hObject    handle to tableGroup (see GCBO)
% eventdata  structure with the following fields (see MATLAB.UI.CONTROL.TABLE)
%	Indices: row and column indices of the cell(s) currently selecteds
% handles    structure with handles and user data (see GUIDATA)
if (~isempty(eventdata.Indices))
    idxFlt = eventdata.Indices(1);
    idxFra = get(handles.editCur, 'string');
    idxFra = str2double(idxFra);
    cntPnt = handles.cntPntAll(idxFra);
    if 1 <= idxFlt && idxFlt <= cntPnt
        drawBfrGrp(handles);
        drawAftGrp(handles);
        drawExtra(handles, idxFlt);
        drawnow;
    end
end
guidata(hObject, handles);


% --- Executes when entered data in editable cell(s) in tableGroup.
function tableGroup_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to tableGroup (see GCBO)
% eventdata  structure with the following fields (see MATLAB.UI.CONTROL.TABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)
IDX_IDM = 7;
idxFlt = eventdata.Indices(1);
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
cntPnt = handles.cntPntAll(idxFra);
if 1 <= idxFlt && idxFlt <= cntPnt
    handles.datPntAll(idxFra, idxFlt, IDX_IDM) = eventdata.NewData;
    drawBfrGrp(handles);
    drawAftGrp(handles);
    drawExtra(handles, idxFlt);
    drawnow;
end
guidata(hObject, handles);


%%*** MY FUNCTION ******************************************************
function logAftGrp(handles)
% parameter
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
IDX_IDO = 5;
IDX_KNL = 6;
IDX_IDM = 7;

% variable
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
cntPnt = handles.cntPntAll(idxFra);
datPnt = ones(cntPnt, IDX_SNR);
idxGrpOri = ones(cntPnt, 1);
idxGrpMod = ones(cntPnt, 1);
datPnt(:,:) = handles.datPntAll(idxFra, 1:cntPnt, 1:IDX_SNR);
idxGrpOri(:,:) = handles.datPntAll(idxFra, 1:cntPnt, IDX_IDO);
idxGrpMod(:,:) = handles.datPntAll(idxFra, 1:cntPnt, IDX_IDM);

% log
set(handles.tableGroup, 'data', [datPnt, idxGrpOri, idxGrpMod]);


function drawBfrGrp(handles)
% parameter
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
IDX_IDO = 5;
IDX_KNL = 6;
IDX_IDM = 7;

% variables
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
cntPnt = handles.cntPntAll(idxFra);
datPnt = ones(cntPnt, IDX_SNR);
datPnt(:,:) = handles.datPntAll(idxFra, 1:cntPnt, 1:IDX_SNR);

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


function drawAftGrp(handles)
% parameter
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
IDX_IDO = 5;
IDX_KNL = 6;
IDX_IDM = 7;

% variables
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
cntPnt = handles.cntPntAll(idxFra);
datPnt = ones(cntPnt, IDX_SNR);
idxGrp = ones(cntPnt, 1);
idxKnl = ones(cntPnt, 1);
datPnt(:,:) = handles.datPntAll(idxFra, 1:cntPnt, 1:IDX_SNR);
%idxGrp(:,:) = handles.datPntAll(idxFra, 1:cntPnt, IDX_IDO);
idxKnl(:,:) = handles.datPntAll(idxFra, 1:cntPnt, IDX_KNL);
idxGrp(:,:) = handles.datPntAll(idxFra, 1:cntPnt, IDX_IDM);

% plot
axes(handles.axesOutput);
cla;
hold on;
for i = 1:max(idxGrp)
    idxFlt = idxGrp == i;
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


function drawExtra(handles, idxFlt)
% parameter
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
IDX_IDO = 5;
IDX_KNL = 6;
IDX_IDM = 7;

% variables
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
cntPnt = handles.cntPntAll(idxFra);
datPnt = ones(cntPnt, IDX_SNR);
datPnt(:,:) = handles.datPntAll(idxFra, 1:cntPnt, 1:IDX_SNR);

% plot
axes(handles.axesInput);
hold on;
plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'xk', 'markersiz', 10);
axes(handles.axesOutput);
hold on;
plot(datPnt(idxFlt,IDX_RNG) .* sin(datPnt(idxFlt,IDX_ANG)), datPnt(idxFlt,IDX_RNG) .* cos(datPnt(idxFlt,IDX_ANG)), 'xk', 'markersiz', 10);


% --- Executes on button press in pushbuttonUp.
function pushbuttonUp_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonUp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
idxFra = min(handles.idxFraMax, max(handles.idxFraMin, idxFra - 1));
set(handles.editCur, 'string', num2str(idxFra));
logAftGrp(handles);
drawBfrGrp(handles);
drawAftGrp(handles);
guidata(hObject, handles);


% --- Executes on button press in pushbuttondump.
function pushbuttondump_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttondump (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% before everything
set(handles.textBusy, 'string', 'busy');

% open file
fileName = get(handles.editFileName, 'string');
fpt = fopen([handles.filePath, fileName, '.group.log'], 'w');
fprintf(fpt, '    %-7s %-7s %-7s %-7s %-2s %-2s\n', 'RNG', 'VEL', 'ANG', 'SNR', 'ID', 'ID (modified)');

% parameter
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
IDX_IDO = 5;
IDX_KNL = 6;
IDX_IDM = 7;

% main loop
for idxFra = handles.idxFraMin:handles.idxFraMax
    % log
    set(handles.editCur, 'string', num2str(idxFra));
    drawnow;

    % get data
    cntPnt = handles.cntPntAll(idxFra);
    datPnt = ones(cntPnt, IDX_SNR);
    idxGrpOri = ones(cntPnt, 1);
    idxGrpMod = ones(cntPnt, 1);
    datPnt(:,:) = handles.datPntAll(idxFra, 1:cntPnt, 1:IDX_SNR);
    idxGrpOri(:,:) = handles.datPntAll(idxFra, 1:cntPnt, IDX_IDO);
    idxGrpMod(:,:) = handles.datPntAll(idxFra, 1:cntPnt, IDX_IDM);

    % dump
    fprintf(fpt, 'BGN OF FRAME %d\n', idxFra);
    fprintf(fpt, '    %-7.2f %-7.2f %-7.2f %-7.2f %02d %02d\n', [datPnt, idxGrpOri, idxGrpMod]');
    fprintf(fpt, 'END OF FRAME %d\n', idxFra);
end

% save
guidata(hObject, handles);

% after everything
set(handles.textBusy, 'string', '');


% --- Executes during object creation, after setting all properties.
function textMin_CreateFcn(hObject, eventdata, handles)
% hObject    handle to textMin (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes during object deletion, before destroying properties.
function textMin_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to textMin (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over textMin.
function textMin_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to textMin (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes during object creation, after setting all properties.
function textMax_CreateFcn(hObject, eventdata, handles)
% hObject    handle to textMax (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes during object deletion, before destroying properties.
function textMax_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to textMax (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function editCur_Callback(hObject, eventdata, handles)
% hObject    handle to editCur (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editCur as text
%        str2double(get(hObject,'String')) returns contents of editCur as a double
idxFra = get(handles.editCur, 'string');
idxFra = str2double(idxFra);
idxFra = min(handles.idxFraMax, max(handles.idxFraMin, idxFra));
set(handles.editCur, 'string', num2str(idxFra));
logAftGrp(handles)
drawBfrGrp(handles);
drawAftGrp(handles);
drawnow;
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function editCur_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editCur (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in radiobuttonTx1.
function radiobuttonTx1_Callback(hObject, eventdata, handles)
% hObject    handle to radiobuttonTx1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobuttonTx1


% --- Executes on button press in radiobuttonTx2.
function radiobuttonTx2_Callback(hObject, eventdata, handles)
% hObject    handle to radiobuttonTx2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobuttonTx2
