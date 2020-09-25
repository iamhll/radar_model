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

% Last Modified by GUIDE v2.5 25-Sep-2020 11:27:18

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


% --- Executes on button press in pushbuttonOpen.
function pushbuttonOpen_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonOpen (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[fileName, filePath] = uigetfile({'*.csv'});
set(handles.editFileName, 'string', fileName)
handles.fptInput = fopen([filePath, fileName], 'r');
handles.fptOutput = fopen([filePath, fileName, '.group.log'], 'w');
handles.datPntLst = [];
guidata(hObject, handles);


% --- Executes on button press in pushbuttonProcess.
function pushbuttonProcess_Callback(hObject, eventdata, handles)
% hObject    handle to pushbuttonProcess (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% get data
% parameter
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;

% save
if isempty(handles.datPntLst)
    handles.numFrame = 1;
else
    fprintf(handles.fptOutput, 'BGN OF FRAME %d\n', handles.numFrame);
    fprintf(handles.fptOutput, '%-7.2f %-7.2f %-7.2f %-7.2f %02d\n', [handles.datPntLst, handles.idxGrp]');
    fprintf(handles.fptOutput, 'END OF FRAME %d\n', handles.numFrame);
    handles.numFrame = handles.numFrame + 1;
end

% init
datPntFul = ones(128, 4);
idxPnt = 1;

% get data
while (1)
    % grep
    datStr = fgetl(handles.fptInput);
    datTokens = regexp(datStr, '^\d+,([0-9.-]+),([0-9.-]+),([0-9.-]+),([0-9.-]+)', 'tokens');
    if ~isempty(datTokens)
        for i = 1:4
            datPntFul(idxPnt, i) = str2double(datTokens{1}{i});
        end
        idxPnt = idxPnt + 1;
    end
    
    % break
    if strcmp(datStr(1:3), 'END')
        break;
    end
end

% filter
%datPntLst = datPntFul(1:idxPnt-1, :);
for i = 1:idxPnt-1
    if datPntFul(i,1) == 1024
        %datPntLst = datPntFul(1:i-1, :);
        datPntLst = datPntFul(i+1:idxPnt-1, :);
        break;
    end
end

% convert
datPntLst(:, IDX_ANG) = datPntLst(:, IDX_ANG) / 180 * pi;

% cluster
datCstLst = pdist2(datPntLst, datPntLst, @cstCustom);
%[idxGrp, idxKnl] = dbscan     (datCstLst, 10, 1, 'distance', 'precomputed');
%[idxGrp, idxKnl] = dbscanMine1(datCstLst, 10, 1);
[idxGrp, idxKnl] = dbscanMine2(datCstLst, 10, 1);
handles.datPntLst = datPntLst;
handles.idxGrp = idxGrp;
handles.idxKnl = idxKnl;

% draw
drawBefore(handles);
drawAfter(handles);
drawnow;

% log
set(handles.tableGroup, 'data', [datPntLst, idxGrp]);
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
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
idxFlt = eventdata.Indices(1);
if 1 <= idxFlt && idxFlt <= size(handles.datPntLst, 1)
    drawBefore(handles);
    drawAfter(handles);
    axes(handles.axesInput);
    plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'xk');
    axes(handles.axesOutput);
    plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'xk');
    drawnow;
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
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
idxFlt = eventdata.Indices(1);
if 1 <= idxFlt && idxFlt <= size(handles.datPntLst, 1)
    handles.idxGrp(idxFlt) = eventdata.NewData;
    drawBefore(handles);
    drawAfter(handles);
    axes(handles.axesInput);
    plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'xk');
    axes(handles.axesOutput);
    plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'xk');
    drawnow;
end
guidata(hObject, handles);


%%*** MY FUNCTION ******************************************************
function drawBefore(handles)
axes(handles.axesInput);
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
cla;
hold on;
% +
idxFlt = handles.datPntLst(:,IDX_VEL) == 0;
plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'og');
% 0
idxFlt = handles.datPntLst(:,IDX_VEL) > 0;
plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'ob');
% -
idxFlt = handles.datPntLst(:,IDX_VEL) < 0;
plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'or');
% set figure
%title('before cluster (color is used to indicate speed)');
title('before cluster');
%axis equal;
axis([-20, 20, 0, 100]);
grid on;

function drawAfter(handles)
IDX_RNG = 1;
IDX_VEL = 2;
IDX_ANG = 3;
IDX_SNR = 4;
axes(handles.axesOutput);
cla;
hold on;
% group
for i = 1:max(handles.idxGrp)
    idxFlt = handles.idxGrp == i;
    plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 'o');
    idxFlt = find(idxFlt, 1);
    text(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)) + 1, handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), num2str(handles.idxGrp(idxFlt)));
end
idxFlt = handles.idxGrp == -1 & handles.idxKnl == 0;
plot(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)), handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), 's');
idxFlt = find(idxFlt, 1);
text(handles.datPntLst(idxFlt,IDX_RNG) .* sin(handles.datPntLst(idxFlt,IDX_ANG)) + 1, handles.datPntLst(idxFlt,IDX_RNG) .* cos(handles.datPntLst(idxFlt,IDX_ANG)), num2str(handles.idxGrp(idxFlt)));
% set figure
title('after cluster');
%axis equal;
axis([-20, 20, 0, 100]);
grid on;
