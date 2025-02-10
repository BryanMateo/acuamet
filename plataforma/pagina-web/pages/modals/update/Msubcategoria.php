
<div class="modal fade text-xs-left" id="update_subcategoria" tabindex="-1" role="dialog" aria-labelledby="myModalLabel2" aria-hidden="true">
	<div class="modal-dialog" role="document">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true">&times;</span>
				</button>
				<h4 class="modal-title" id="myModalLabel2"><i class="icon-road2"></i> Editar subcategoria</h4>
			</div>
			<div class="modal-body">
                <div class="form-group">
                    <label for="nombre_cat"> Nombre de la sub categoria</label>
                    <div class="position-relative has-icon-left">
                        <input type="text" class="form-control" id="nombre_subcat_edit" autofocus>
                        <div class="form-control-position"><i class="icon-bag"></i></div>
                    </div>
                </div>
                <div class="form-group">
                    <label for="descripcion_cat"> Categoria Perteneciente</label>
                    <div class="position-relative has-icon-left">
						<select class="form-control" id="categoria_perteneciente_edit"></select>
						<div class="form-control-position"><i class="icon-list"></i></div>
                    </div>
                </div>
			</div>
			<div class="modal-footer">
				<button type="button" class="btn grey btn-outline-secondary" data-dismiss="modal">Cancelar</button>
				<button type="button" class="btn btn-outline-primary" onclick="">Guardar cambios</button>
			</div>
		</div>
	</div>
</div>